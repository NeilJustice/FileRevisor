#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileOpenerCloserMock.h"

TESTS(DirectoryIteratorTests)
AFACT(DefaultConstructor_NewsComponents_SetsFieldsToDefaultValues)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsEndIterationMarker_ReturnsEmptyVector)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsNotEndIterationMarker_FileIsSkippableType_DoesNotAddToTextFilePaths_ReturnsEmptyVector)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsNotEndIterationMarker_FileIsNotSkippableType_AddsToTextFilePaths_ReturnsTextFilePaths)
FACTS(PathContainsAnySubstringCaseInsensitive_ReturnsTrueIfFilePathCaseInsensitiveContainsAnySubstring)
AFACT(SetDirectoryIterator_RecurseIsTrue_SetsRecursiveDirectoryIterator_SetsRecursiveModeToTrue)
AFACT(SetDirectoryIterator_RecurseIsFalse_SetsDirectoryIterator)
AFACT(SetFileAndDirectoryPathIgnoreSubstrings_SetsFileAndDirectoryPathIgnoreSubstringsVector)
EVIDENCE

DirectoryIterator _directoryIterator;
// Constant Components
ConsoleMock* _consoleMock = nullptr;
FileOpenerCloserMock* _fileOpenerCloserMock = nullptr;

class DirectoryIteratorSelfMocked : public Metal::Mock<DirectoryIterator>
{
public:
   METALMOCK_NONVOID0(fs::path, NextNonIgnoredFilePath)
   METALMOCK_NONVOID1_CONST(bool, IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable, const fs::path&)
};
DirectoryIteratorSelfMocked _directoryIteratorSelfMocked;

STARTUP
{
   // Constant Components
   _directoryIterator._console.reset(_consoleMock = new ConsoleMock);
   _directoryIterator._fileOpenerCloser.reset(_fileOpenerCloserMock = new FileOpenerCloserMock);
}

TEST(DefaultConstructor_NewsComponents_SetsFieldsToDefaultValues)
{
   DirectoryIterator directoryIterator;
   // Constant Components
   DELETE_TO_ASSERT_NEWED(directoryIterator._console);
   DELETE_TO_ASSERT_NEWED(directoryIterator._fileOpenerCloser);
   // Mutable Fields
	ARE_EQUAL(fs::directory_iterator(), directoryIterator._directoryIterator);
   ARE_EQUAL(fs::recursive_directory_iterator(), directoryIterator._recursiveDirectoryIterator);
   IS_EMPTY(directoryIterator._fileAndDirectoryPathIgnoreSubstrings);
	IS_FALSE(directoryIterator._recursiveMode);
}

TEST(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsEndIterationMarker_ReturnsEmptyVector)
{
   const fs::path endIterationMarker;
   _directoryIteratorSelfMocked.NextNonIgnoredFilePathMock.Return(endIterationMarker);
   //
   const vector<fs::path> textFilePaths = _directoryIteratorSelfMocked.GetNonEmptyNonIgnoredTextFilePaths();
   //
   METALMOCK(_directoryIteratorSelfMocked.NextNonIgnoredFilePathMock.CalledOnce());
   IS_EMPTY(textFilePaths);
}

TEST(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsNotEndIterationMarker_FileIsSkippableType_DoesNotAddToTextFilePaths_ReturnsEmptyVector)
{
   const fs::path firstFilePath = ZenUnit::Random<fs::path>();
   const fs::path endIterationMarker{};
   _directoryIteratorSelfMocked.NextNonIgnoredFilePathMock.ReturnValues(firstFilePath, endIterationMarker);
   _directoryIteratorSelfMocked.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenableMock.Return(true);
   //
   const vector<fs::path> textFilePaths = _directoryIteratorSelfMocked.GetNonEmptyNonIgnoredTextFilePaths();
   //
   METALMOCK(_directoryIteratorSelfMocked.NextNonIgnoredFilePathMock.CalledNTimes(2));
   METALMOCK(_directoryIteratorSelfMocked.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenableMock.CalledOnceWith(firstFilePath));
   IS_EMPTY(textFilePaths);
}

TEST(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsNotEndIterationMarker_FileIsNotSkippableType_AddsToTextFilePaths_ReturnsTextFilePaths)
{
   const fs::path firstFilePath = ZenUnit::Random<fs::path>();
   const fs::path secondFilePath = ZenUnit::Random<fs::path>();
   const fs::path thirdFilePath = ZenUnit::Random<fs::path>();
   const fs::path endIterationMarker{};
   _directoryIteratorSelfMocked.NextNonIgnoredFilePathMock.ReturnValues(firstFilePath, secondFilePath, thirdFilePath, endIterationMarker);
   _directoryIteratorSelfMocked.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenableMock.ReturnValues(false, true, false);
   //
   const vector<fs::path> textFilePaths = _directoryIteratorSelfMocked.GetNonEmptyNonIgnoredTextFilePaths();
   //
   METALMOCK(_directoryIteratorSelfMocked.NextNonIgnoredFilePathMock.CalledNTimes(4));
   METALMOCK(_directoryIteratorSelfMocked.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenableMock.CalledAsFollows(
   {
      firstFilePath,
      secondFilePath,
      thirdFilePath
   }));
   const vector<fs::path> expectedTextFilePaths =
   {
      firstFilePath,
      thirdFilePath
   };
   VECTORS_ARE_EQUAL(expectedTextFilePaths, textFilePaths);
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
   bool pathContainsAnySubstringCaseInsensitive = DirectoryIterator::PathContainsAnySubstringCaseInsensitive(path, substrings);
   ARE_EQUAL(expectedReturnValue, pathContainsAnySubstringCaseInsensitive);
}

TEST(SetDirectoryIterator_RecurseIsTrue_SetsRecursiveDirectoryIterator_SetsRecursiveModeToTrue)
{
   //const fs::path directoryPath = ZenUnit::Random<fs::path>();
   ////
   //_directoryIterator.SetDirectoryIterator(directoryPath, true);
   ////
   //IS_TRUE(_directoryIterator._recursiveMode);
}

TEST(SetDirectoryIterator_RecurseIsFalse_SetsDirectoryIterator)
{
   //const fs::path directoryPath = ZenUnit::Random<fs::path>();
   ////
   //_directoryIterator.SetDirectoryIterator(directoryPath, false);
   ////
   //IS_TRUE(_directoryIterator._recursiveMode);
}

TEST(SetFileAndDirectoryPathIgnoreSubstrings_SetsFileAndDirectoryPathIgnoreSubstringsVector)
{
   const vector<string> fileAndDirectoryPathIgnoreSubstrings = ZenUnit::RandomVector<string>();
   //
   _directoryIterator.SetFileAndDirectoryPathIgnoreSubstrings(fileAndDirectoryPathIgnoreSubstrings);
   //
   VECTORS_ARE_EQUAL(fileAndDirectoryPathIgnoreSubstrings, _directoryIterator._fileAndDirectoryPathIgnoreSubstrings);
}

RUN_TESTS(DirectoryIteratorTests)
