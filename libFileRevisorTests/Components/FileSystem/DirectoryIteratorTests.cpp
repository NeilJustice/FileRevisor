#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisorTests/UtilityComponents/DataStructures/MetalMock/CharArray64HelperMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileOpenerCloserMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileReaderMock.h"

TESTS(DirectoryIteratorTests)
AFACT(DefaultConstructor_NewsComponents_SetsFieldsToDefaultValues)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsEndIterationMarker_ReturnsEmptyVector)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsNotEndIterationMarker_FileIsSkippableType_DoesNotAddToTextFilePaths_ReturnsEmptyVector)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsNotEndIterationMarker_FileIsNotSkippableType_AddsToTextFilePaths_ReturnsTextFilePaths)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNullptr_WritesErrorMessage_ReturnsTrue)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst64BytesOfFile_FileLengthIs0_ReturnsTrue)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst64BytesOfFile_FileLengthIsNot0_First64BytesContains0_ReturnsTrue)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst64BytesOfFile_FileLengthIsNot0_First64BytesDoesNotContain0_ReturnsFalse)
FACTS(PathContainsAnySubstringCaseInsensitive_ReturnsTrueIfFilePathCaseInsensitiveContainsAnySubstring)
AFACT(SetFileAndDirectoryPathIgnoreSubstrings_SetsFileAndDirectoryPathIgnoreSubstringsVector)
EVIDENCE

DirectoryIterator _directoryIterator;
// Constant Components
CharArray64HelperMock* _charArray64HelperMock = nullptr;
ConsoleMock* _consoleMock = nullptr;
FileOpenerCloserMock* _fileOpenerCloserMock = nullptr;
FileReaderMock* _fileReaderMock = nullptr;

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
   _directoryIterator._charArray64Helper.reset(_charArray64HelperMock = new CharArray64HelperMock);
   _directoryIterator._console.reset(_consoleMock = new ConsoleMock);
   _directoryIterator._fileOpenerCloser.reset(_fileOpenerCloserMock = new FileOpenerCloserMock);
   _directoryIterator._fileReader.reset(_fileReaderMock = new FileReaderMock);
}

TEST(DefaultConstructor_NewsComponents_SetsFieldsToDefaultValues)
{
   DirectoryIterator directoryIterator;
   // Constant Components
   DELETE_TO_ASSERT_NEWED(directoryIterator._charArray64Helper);
   DELETE_TO_ASSERT_NEWED(directoryIterator._console);
   DELETE_TO_ASSERT_NEWED(directoryIterator._fileOpenerCloser);
   DELETE_TO_ASSERT_NEWED(directoryIterator._fileReader);
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

TEST(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst64BytesOfFile_FileLengthIs0_ReturnsTrue)
{
   const shared_ptr<FILE> fileOpenInBinaryReadMode = make_shared<FILE>();
   _fileOpenerCloserMock->OpenReadModeBinaryFileMock.Return(fileOpenInBinaryReadMode);

   const pair<size_t, array<char, 64>> fileIsEmptyAndFirst64Bytes = { 0, {} };
   _fileReaderMock->ReadFirst64BytesMock.Return(fileIsEmptyAndFirst64Bytes);

   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const bool isFileEmptyOrBinaryOrNotAnsiOrNotOpenable = _directoryIterator.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(filePath);
   //
   METALMOCK(_fileOpenerCloserMock->OpenReadModeBinaryFileMock.CalledOnceWith(filePath, false));
   METALMOCK(_fileReaderMock->ReadFirst64BytesMock.CalledOnceWith(fileOpenInBinaryReadMode.get()));
   IS_TRUE(isFileEmptyOrBinaryOrNotAnsiOrNotOpenable);
}

TEST(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst64BytesOfFile_FileLengthIsNot0_First64BytesContains0_ReturnsTrue)
{
   const shared_ptr<FILE> fileOpenInBinaryReadMode = make_shared<FILE>();
   _fileOpenerCloserMock->OpenReadModeBinaryFileMock.Return(fileOpenInBinaryReadMode);

   const size_t numberOfBytesRead = ZenUnit::RandomNon0<size_t>();
   const pair<size_t, array<char, 64>> fileIsEmptyAndFirst64Bytes = { numberOfBytesRead, {} };
   _fileReaderMock->ReadFirst64BytesMock.Return(fileIsEmptyAndFirst64Bytes);

   _charArray64HelperMock->ArrayContains0Mock.Return(true);

   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const bool isFileEmptyOrBinaryOrNotAnsiOrNotOpenable = _directoryIterator.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(filePath);
   //
   METALMOCK(_fileOpenerCloserMock->OpenReadModeBinaryFileMock.CalledOnceWith(filePath, false));
   METALMOCK(_fileReaderMock->ReadFirst64BytesMock.CalledOnceWith(fileOpenInBinaryReadMode.get()));
   METALMOCK(_charArray64HelperMock->ArrayContains0Mock.CalledOnceWith(fileIsEmptyAndFirst64Bytes.second, numberOfBytesRead));
   IS_TRUE(isFileEmptyOrBinaryOrNotAnsiOrNotOpenable);
}

TEST(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst64BytesOfFile_FileLengthIsNot0_First64BytesDoesNotContain0_ReturnsFalse)
{
   const shared_ptr<FILE> fileOpenInBinaryReadMode = make_shared<FILE>();
   _fileOpenerCloserMock->OpenReadModeBinaryFileMock.Return(fileOpenInBinaryReadMode);

   const size_t numberOfBytesRead = ZenUnit::RandomNon0<size_t>();
   const pair<size_t, array<char, 64>> fileIsEmptyAndFirst64Bytes = { numberOfBytesRead, {} };
   _fileReaderMock->ReadFirst64BytesMock.Return(fileIsEmptyAndFirst64Bytes);

   _charArray64HelperMock->ArrayContains0Mock.Return(false);

   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const bool isFileEmptyOrBinaryOrNotAnsiOrNotOpenable = _directoryIterator.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(filePath);
   //
   METALMOCK(_fileOpenerCloserMock->OpenReadModeBinaryFileMock.CalledOnceWith(filePath, false));
   METALMOCK(_fileReaderMock->ReadFirst64BytesMock.CalledOnceWith(fileOpenInBinaryReadMode.get()));
   METALMOCK(_charArray64HelperMock->ArrayContains0Mock.CalledOnceWith(fileIsEmptyAndFirst64Bytes.second, numberOfBytesRead));
   IS_FALSE(isFileEmptyOrBinaryOrNotAnsiOrNotOpenable);
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

TEST(SetFileAndDirectoryPathIgnoreSubstrings_SetsFileAndDirectoryPathIgnoreSubstringsVector)
{
   const vector<string> fileAndDirectoryPathIgnoreSubstrings = ZenUnit::RandomVector<string>();
   //
   _directoryIterator.SetFileAndDirectoryPathIgnoreSubstrings(fileAndDirectoryPathIgnoreSubstrings);
   //
   VECTORS_ARE_EQUAL(fileAndDirectoryPathIgnoreSubstrings, _directoryIterator._fileAndDirectoryPathIgnoreSubstrings);
}

RUN_TESTS(DirectoryIteratorTests)
