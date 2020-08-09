#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"

TESTS(DirectoryIteratorTests)
AFACT(DefaultConstructor_SetsFieldsToDefaultValues)
FACTS(PathContainsAnySubstringCaseInsensitive_ReturnsTrueIfFilePathCaseInsensitiveContainsAnySubstring)
EVIDENCE

DirectoryIterator _directoryIterator;

TEST(DefaultConstructor_SetsFieldsToDefaultValues)
{
   DirectoryIterator directoryIterator;
	ARE_EQUAL(fs::directory_iterator(), directoryIterator._directoryIterator);
   ARE_EQUAL(fs::recursive_directory_iterator(), directoryIterator._recursiveDirectoryIterator);
   IS_EMPTY(directoryIterator._fileAndDirectoryPathIgnoreSubstrings);
	IS_FALSE(directoryIterator._recursiveMode);
}

TEST3X3(PathContainsAnySubstringCaseInsensitive_ReturnsTrueIfFilePathCaseInsensitiveContainsAnySubstring,
   const fs::path& path, const vector<string>& substrings, bool expectedReturnValue,
   "", vector<string>{}, false,
   "a", vector<string>{"a"}, true,
   "A", vector<string>{"a"}, true,
   "a", vector<string>{"b", "a"}, true,
   "abc bb def", vector<string>{"aa", "bb", "cc"}, true,
   "C:\\Directory\\SubdirectoryC\\FileName.ext", vector<string>{"subdirectoryA\\", "subdirectoryB\\", "subdirectoryC\\"}, true,
   "C:\\Directory\\SubdirectoryC", vector<string>{"subdirectoryA\\", "subdirectoryB\\", "subdirectoryC\\"}, false)
{
   bool pathContainsAnySubstringCaseInsensitive = _directoryIterator.PathContainsAnySubstringCaseInsensitive(path, substrings);
   ARE_EQUAL(expectedReturnValue, pathContainsAnySubstringCaseInsensitive);
}

RUN_TESTS(DirectoryIteratorTests)
