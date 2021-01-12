#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"

DirectoryIterator::DirectoryIterator() noexcept
   // Constant Components
   : _fileOpenerCloser(make_unique<FileOpenerCloser>())
   // Mutable Fields
	, _recursiveMode(false)
{
}

vector<fs::path> DirectoryIterator::GetNonEmptyNonIgnoredTextFilePaths(bool /*skipFilesInUse*/)
{
   vector<fs::path> textFilePaths;
   while (true)
   {
      fs::path nextNonIgnoredFilePath = NextNonIgnoredFilePath();
      static const fs::path endIterationMarker{};
      if (nextNonIgnoredFilePath == endIterationMarker)
      {
         break;
      }
      const bool fileIsEmptyOrBinaryOrNonAnsi = IsFileEmptyOrBinaryOrNonAnsi(nextNonIgnoredFilePath);
      if (fileIsEmptyOrBinaryOrNonAnsi)
      {
         continue;
      }
      textFilePaths.emplace_back(std::move(nextNonIgnoredFilePath));
   }
   return textFilePaths;
}

fs::path DirectoryIterator::NextNonIgnoredDirectoryPath()
{
   fs::path nextNonIgnoredDirectoryPath;
   if (_recursiveMode)
   {
      nextNonIgnoredDirectoryPath = NextNonIgnoredPath(_recursiveDirectoryIterator, fs::file_type::directory);
   }
   else
   {
      nextNonIgnoredDirectoryPath = NextNonIgnoredPath(_directoryIterator, fs::file_type::directory);
   }
   return nextNonIgnoredDirectoryPath;
}

fs::path DirectoryIterator::NextNonIgnoredFilePath()
{
   fs::path nextNonIgnoredFilePath;
   if (_recursiveMode)
   {
      nextNonIgnoredFilePath = NextNonIgnoredPath(_recursiveDirectoryIterator, fs::file_type::regular);
   }
   else
   {
      nextNonIgnoredFilePath = NextNonIgnoredPath(_directoryIterator, fs::file_type::regular);
   }
   return nextNonIgnoredFilePath;
}

void DirectoryIterator::SetDirectoryIterator(const fs::path& directoryPath, bool recurse)
{
   if (recurse)
   {
      _recursiveDirectoryIterator = fs::recursive_directory_iterator(directoryPath);
      _recursiveMode = true;
   }
   else
   {
      _directoryIterator = fs::directory_iterator(directoryPath);
   }
}

void DirectoryIterator::SetFileAndDirectoryPathIgnoreSubstrings(const vector<string>& fileAndDirectoryPathIgnoreSubstrings)
{
   _fileAndDirectoryPathIgnoreSubstrings = fileAndDirectoryPathIgnoreSubstrings;
}

// Private Functions

bool DirectoryIterator::IsFileEmptyOrBinaryOrNonAnsi(const fs::path& filePath) const
{
   FILE* const fileOpenInReadBinaryMode = _fileOpenerCloser->OpenReadModeBinaryFile(filePath);
   char first1KBytesInFile[1024];
#if defined __linux__|| defined __APPLE__
   const size_t numberOfBytesRead = fread(
      first1KBytesInFile, 1, sizeof(first1KBytesInFile), fileOpenInReadBinaryMode);
#else
   const size_t numberOfBytesRead = _fread_nolock_s(
      first1KBytesInFile, sizeof(first1KBytesInFile), 1, sizeof(first1KBytesInFile), fileOpenInReadBinaryMode);
#endif
   _fileOpenerCloser->CloseFile(fileOpenInReadBinaryMode, filePath);
   if (numberOfBytesRead == 0)
   {
      return true;
   }
   for (size_t i = 0; i < numberOfBytesRead; ++i)
   {
      const char b = first1KBytesInFile[i];
      if (b == 0)
      {
         return true;
      }
   }
   return false;
}

template<typename DirectoryIteratorType>
fs::path DirectoryIterator::NextNonIgnoredPath(DirectoryIteratorType& iter, fs::file_type requiredFileType)
{
   static const DirectoryIteratorType noMorePathsMarker{};
   while (true)
   {
      if (iter == noMorePathsMarker)
      {
         return fs::path();
      }
      const fs::directory_entry& nextDirectoryEntry = *iter++;
      const fs::file_status nextDirectoryEntrySymlinkStatus = nextDirectoryEntry.symlink_status();
      const fs::file_type nextDirectoryEntrySymlinkStatusType = nextDirectoryEntrySymlinkStatus.type();
      const bool isSymlink = nextDirectoryEntrySymlinkStatusType == fs::file_type::symlink;
      if (isSymlink)
      {
         continue;
      }
      const fs::file_status nextDirectoryEntryStatus = nextDirectoryEntry.status();
      const fs::file_type nextDirectoryEntryStatusType = nextDirectoryEntryStatus.type();
      if (nextDirectoryEntryStatusType == requiredFileType)
      {
         fs::path nextPathWithMatchingFileType = nextDirectoryEntry.path();
         const bool pathIsIgnored = PathContainsAnySubstringCaseInsensitive(
            nextPathWithMatchingFileType, _fileAndDirectoryPathIgnoreSubstrings);
         if (pathIsIgnored)
         {
            continue;
         }
         return nextPathWithMatchingFileType;
      }
   }
}

bool DirectoryIterator::PathContainsAnySubstringCaseInsensitive(const fs::path& path, const vector<string>& pathSubstrings)
{
   if (pathSubstrings.empty())
   {
      return false;
   }
   const string pathAsString = path.string();
   const bool doIgnorePath = std::any_of(begin(pathSubstrings), end(pathSubstrings), [&](const string& ignoredSubstring)
   {
      if (String::CaseInsensitiveContainsSubstring(pathAsString, ignoredSubstring))
      {
         return true;
      }
      return false;
   });
   return doIgnorePath;
}
