#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileOpenerCloserMock.h"

TESTS(DirectoryIteratorTests)
AFACT(DefaultConstructor_NewsComponents_SetsFieldsToDefaultValues)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsEndIterationMarker_ReturnsEmptyVector)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsNotEndIterationMarker_FileIsSkippableType_DoesNotAddToTextFilePaths_ReturnsEmptyVector)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsNotEndIterationMarker_FileIsNotSkippableType_AddsToTextFilePaths_ReturnsTextFilePaths)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNullptr_WritesErrorMessage_ReturnsTrue)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst1KBOfFile_Reads0Bytes_ReturnsTrue)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst1KBOfFile_ReadsNot0Bytes_First1KBBytesContains0_ReturnsTrue)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst1KBOfFile_ReadsNot0Bytes_First1KBBytesDoesNotContain0_ReturnsFalse)
FACTS(PathContainsAnySubstringCaseInsensitive_ReturnsTrueIfFilePathCaseInsensitiveContainsAnySubstring)
AFACT(SetDirectoryIterator_RecurseIsTrue_SetsRecursiveDirectoryIterator_SetsRecursiveModeToTrue)
AFACT(SetDirectoryIterator_RecurseIsFalse_SetsDirectoryIterator)
AFACT(SetFileAndDirectoryPathIgnoreSubstrings_SetsFileAndDirectoryPathIgnoreSubstringsVector)
EVIDENCE

DirectoryIterator _directoryIterator;
// Function Pointers
#if defined __linux__
METALMOCK_NONVOID4_FREE(size_t, _call_fread, void*, size_t, size_t, FILE*)
#elif defined _WIN32
METALMOCK_NONVOID5_FREE(size_t, _call_fread_nolock_s, void*, size_t, size_t, size_t, FILE*)
#endif
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
   // Function Pointers
#if defined __linux__
   _directoryIterator._call_fread = BIND_4ARG_METALMOCK_OBJECT(_call_freadMock);
#elif defined _WIN32
   _directoryIterator._call_fread_nolock_s = BIND_5ARG_METALMOCK_OBJECT(_call_fread_nolock_sMock);
#endif
   // Constant Components
   _directoryIterator._console.reset(_consoleMock = new ConsoleMock);
   _directoryIterator._fileOpenerCloser.reset(_fileOpenerCloserMock = new FileOpenerCloserMock);
}

TEST(DefaultConstructor_NewsComponents_SetsFieldsToDefaultValues)
{
   DirectoryIterator directoryIterator;
   // Function Pointers
#if defined __linux__
   STD_FUNCTION_TARGETS(fread, _directoryIterator._call_fread);
#elif defined _WIN32
   STD_FUNCTION_TARGETS(_fread_nolock_s, directoryIterator._call_fread_nolock_s);
#endif
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

TEST(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNullptr_WritesErrorMessage_ReturnsTrue)
{
   _fileOpenerCloserMock->OpenReadModeBinaryFileMock.Return(nullptr);
   _consoleMock->WriteLineMock.Expect();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const bool isFileEmptyOrBinaryOrNotAnsiOrNotOpenable = _directoryIterator.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(filePath);
   //
   METALMOCK(_fileOpenerCloserMock->OpenReadModeBinaryFileMock.CalledOnceWith(filePath, false));
   const string expectedUnableToOpenFileMessage = String::ConcatStrings("[FileRevisor]     Note: Unable to open file ", filePath.string());
   METALMOCK(_consoleMock->WriteLineMock.CalledOnceWith(expectedUnableToOpenFileMessage));
   IS_TRUE(isFileEmptyOrBinaryOrNotAnsiOrNotOpenable);
}

TEST(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst1KBOfFile_Reads0Bytes_ReturnsTrue)
{
   //FILE fileOpenInReadBinaryMode{};
   //_fileOpenerCloserMock->OpenReadModeBinaryFileMock.Return(&fileOpenInReadBinaryMode);
   //_fileOpenerCloserMock->CloseFileMock.Expect();
   //const fs::path filePath = ZenUnit::Random<fs::path>();
   ////
   //_directoryIterator.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(filePath);
   ////
   //METALMOCK(_fileOpenerCloserMock->OpenReadModeBinaryFileMock.CalledOnceWith(filePath, false));
   //METALMOCK(_fileOpenerCloserMock->CloseFileMock.CalledOnceWith(&fileOpenInReadBinaryMode, filePath));
}

TEST(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst1KBOfFile_ReadsNot0Bytes_First1KBBytesContains0_ReturnsTrue)
{

}

TEST(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst1KBOfFile_ReadsNot0Bytes_First1KBBytesDoesNotContain0_ReturnsFalse)
{

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
