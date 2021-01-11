#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"

DirectoryIterator::DirectoryIterator() noexcept
	: _recursiveMode(false)
{
}

void DirectoryIterator::SetFileAndDirectoryPathIgnoreSubstrings(const vector<string>& fileAndDirectoryPathIgnoreSubstrings)
{
   _fileAndDirectoryPathIgnoreSubstrings = fileAndDirectoryPathIgnoreSubstrings;
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
