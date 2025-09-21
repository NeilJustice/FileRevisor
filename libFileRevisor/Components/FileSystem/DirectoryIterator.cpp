#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"
#include "libFileRevisor/Components/FileSystem/FileReader.h"
#include "libFileRevisor/Components/DataStructures/CharArray256Helper.h"

DirectoryIterator::DirectoryIterator() noexcept
   // Constant Components
   : _charArray256Helper(make_unique<CharArray256Helper>())
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

fs::path DirectoryIterator::NextNonIgnoredFolderPath()
{
   fs::path nextNonIgnoredFolderPath;
   if (_recursiveMode)
   {
      nextNonIgnoredFolderPath = NextNonIgnoredPath(_recursiveDirectoryIterator, fs::file_type::directory);
   }
   else
   {
      nextNonIgnoredFolderPath = NextNonIgnoredPath(_directoryIterator, fs::file_type::directory);
   }
   return nextNonIgnoredFolderPath;
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

void DirectoryIterator::SetFileAndFolderPathIgnoreSubstrings(const vector<string>& fileAndFolderPathIgnoreSubstrings)
{
   _fileAndFolderPathIgnoreSubstrings = fileAndFolderPathIgnoreSubstrings;
}

// Private Functions

bool DirectoryIterator::IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(const fs::path& filePath) const
{
   const shared_ptr<FILE> fileOpenInBinaryReadMode = _fileOpenerCloser->OpenReadModeBinaryFile(filePath, false);
   if (fileOpenInBinaryReadMode == nullptr)
   {
      const string unableToOpenFileMessage = String::ConcatStrings("Note: Unable to open file ", filePath.string());
      _console->ProgramNameThreadIdWriteLineColor(unableToOpenFileMessage, Color::Yellow);
      return true;
   }
   const pair<size_t, array<char, 256>> fileIsEmptyAndFirst256Bytes = _fileReader->ReadFirst256Bytes(fileOpenInBinaryReadMode.get());
   const size_t numberOfBytesRead = fileIsEmptyAndFirst256Bytes.first;
   if (numberOfBytesRead == 0)
   {
      release_assert(fileIsEmptyAndFirst256Bytes.second[0] == 0);
      return true;
   }
   const bool first256BytesContain0 = _charArray256Helper->ArrayContains0(fileIsEmptyAndFirst256Bytes.second, numberOfBytesRead);
   if (first256BytesContain0)
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
         const bool pathIsIgnored = PathContainsAnySubstringCaseInsensitive(nextPathWithMatchingFileType, _fileAndFolderPathIgnoreSubstrings);
         if (pathIsIgnored)
         {
            continue;
         }
         return nextPathWithMatchingFileType;
      }
   }
}

bool DirectoryIterator::PathContainsAnySubstringCaseInsensitive(const fs::path& fileOrFolderPath, const vector<string>& pathSubstrings)
{
   if (pathSubstrings.empty())
   {
      return false;
   }
   const string fileOrFolderPathAsString = fileOrFolderPath.string();
   const bool doIgnoreFileOrFolderPath = std::any_of(pathSubstrings.cbegin(), pathSubstrings.cend(), [&](const string& ignoredSubstring)
   {
      if (String::CaseInsensitiveContainsSubstring(fileOrFolderPathAsString, ignoredSubstring))
      {
         return true;
      }
      return false;
   });
   return doIgnoreFileOrFolderPath;
}
