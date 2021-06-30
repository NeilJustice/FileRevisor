#include "pch.h"
#include "libFileRevisor/UtilityComponents/DataStructures/CharArray64Helper.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"
#include "libFileRevisor/Components/FileSystem/FileReader.h"

DirectoryIterator::DirectoryIterator() noexcept
   // Constant Components
   : _charArray64Helper(make_unique<CharArray64Helper>())
   , _console(make_unique<Console>())
   , _fileOpenerCloser(make_unique<FileOpenerCloser>())
   , _fileReader(make_unique<FileReader>())
   // Mutable Fields
	, _recursiveMode(false)
{
}

DirectoryIterator::~DirectoryIterator()
{
}

vector<fs::path> DirectoryIterator::GetNonEmptyNonIgnoredTextFilePaths()
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
      const bool fileIsEmptyOrBinaryOrNotAnsiOrNotOpenable = IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(nextNonIgnoredFilePath);
      if (fileIsEmptyOrBinaryOrNotAnsiOrNotOpenable)
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

bool DirectoryIterator::IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(const fs::path& filePath) const
{
   const shared_ptr<FILE> fileOpenInBinaryReadMode = _fileOpenerCloser->OpenReadModeBinaryFile(filePath, false);
   if (fileOpenInBinaryReadMode == nullptr)
   {
      const string unableToOpenFileMessage = String::ConcatStrings("[FileRevisor]     Note: Unable to open file ", filePath.string());
      _console->WriteLine(unableToOpenFileMessage);
      return true;
   }
   const pair<size_t, array<char, 64>> fileIsEmptyAndFirst64Bytes = _fileReader->ReadFirst64Bytes(fileOpenInBinaryReadMode.get());
   const size_t numberOfBytesRead = fileIsEmptyAndFirst64Bytes.first;
   if (numberOfBytesRead == 0)
   {
      return true;
   }
   const bool first64BytesContains0 = _charArray64Helper->ArrayContains(fileIsEmptyAndFirst64Bytes.second, 0, numberOfBytesRead);
   if (first64BytesContains0)
   {
      return true;
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
