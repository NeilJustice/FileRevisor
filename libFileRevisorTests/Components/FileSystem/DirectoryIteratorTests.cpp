#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisorTests/Components/DataStructures/MetalMock/CharArray256HelperMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileOpenerCloserMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileReaderMock.h"

TESTS(DirectoryIteratorTests)
AFACT(DefaultConstructor_SetsFieldsToDefaultValues)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsEndIterationMarker_ReturnsEmptyVector)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsNotEndIterationMarker_FileIsSkippableType_DoesNotAddToTextFilePaths_ReturnsEmptyVector)
AFACT(GetNonEmptyNonIgnoredTextFilePaths_NextNonIgnoredFilePathIsNotEndIterationMarker_FileIsNotSkippableType_AddsToTextFilePaths_ReturnsTextFilePaths)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNullptr_WritesYellowNoteMessage_ReturnsTrue)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst256BytesOfFile_FileLengthIs0_ReturnsTrue)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst256BytesOfFile_FileLengthIsNot0_First256BytesContains0_ReturnsTrue)
AFACT(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst256BytesOfFile_FileLengthIsNot0_First256BytesDoesNotContain0_ReturnsFalse)
FACTS(PathContainsAnySubstringCaseInsensitive_ReturnsTrueIfFilePathCaseInsensitiveContainsAnySubstring)
AFACT(SetFileAndFolderPathIgnoreSubstrings_SetsFileAndFolderPathIgnoreSubstringsVector)
EVIDENCE

DirectoryIterator _directoryIterator;
// Constant Components
CharArray256HelperMock* _charArray256HelperMock = nullptr;
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
   _directoryIterator._charArray256Helper.reset(_charArray256HelperMock = new CharArray256HelperMock);
   _directoryIterator._console.reset(_consoleMock = new ConsoleMock);
   _directoryIterator._fileOpenerCloser.reset(_fileOpenerCloserMock = new FileOpenerCloserMock);
   _directoryIterator._fileReader.reset(_fileReaderMock = new FileReaderMock);
}

TEST(DefaultConstructor_SetsFieldsToDefaultValues)
{
   const DirectoryIterator directoryIterator;
   // Mutable Fields
   ARE_EQUAL(fs::directory_iterator(), directoryIterator._directoryIterator);
   ARE_EQUAL(fs::recursive_directory_iterator(), directoryIterator._recursiveDirectoryIterator);
   IS_EMPTY(directoryIterator._fileAndFolderPathIgnoreSubstrings);
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
   const fs::path endIterationMarker;
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
   const fs::path endIterationMarker;
   _directoryIteratorSelfMocked.NextNonIgnoredFilePathMock.ReturnValues(
      firstFilePath,
      secondFilePath,
      thirdFilePath,
      endIterationMarker);

   _directoryIteratorSelfMocked.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenableMock.ReturnValues(false, true, false);
   //
   const vector<fs::path> textFilePaths = _directoryIteratorSelfMocked.GetNonEmptyNonIgnoredTextFilePaths();
   //
   METALMOCK(_directoryIteratorSelfMocked.NextNonIgnoredFilePathMock.CalledNTimes(4));
   METALMOCK(_directoryIteratorSelfMocked.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenableMock.CalledNTimes(3));
   METALMOCKTHEN(_directoryIteratorSelfMocked.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenableMock.CalledWith(firstFilePath)).Then(
   METALMOCKTHEN(_directoryIteratorSelfMocked.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenableMock.CalledWith(secondFilePath))).Then(
   METALMOCKTHEN(_directoryIteratorSelfMocked.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenableMock.CalledWith(thirdFilePath)));
   const vector<fs::path> expectedTextFilePaths =
   {
      firstFilePath,
      thirdFilePath
   };
   VECTORS_ARE_EQUAL(expectedTextFilePaths, textFilePaths);
}

TEST(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNullptr_WritesYellowNoteMessage_ReturnsTrue)
{
   _fileOpenerCloserMock->OpenReadModeBinaryFileMock.Return(nullptr);
   _consoleMock->ProgramNameThreadIdWriteLineColorMock.Expect();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const bool isFileEmptyOrBinaryOrNotAnsiOrNotOpenable = _directoryIterator.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(filePath);
   //
   const string expectedUnableToOpenFileMessage = Utils::String::ConcatStrings("Note: Unable to open file ", filePath.string());
   METALMOCKTHEN(_fileOpenerCloserMock->OpenReadModeBinaryFileMock.CalledOnceWith(filePath, false)).Then(
   METALMOCKTHEN(_consoleMock->ProgramNameThreadIdWriteLineColorMock.CalledOnceWith(expectedUnableToOpenFileMessage, Color::Yellow)));
   IS_TRUE(isFileEmptyOrBinaryOrNotAnsiOrNotOpenable);
}

TEST(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst256BytesOfFile_FileLengthIs0_ReturnsTrue)
{
   const shared_ptr<FILE> fileOpenInBinaryReadMode = make_shared<FILE>();
   _fileOpenerCloserMock->OpenReadModeBinaryFileMock.Return(fileOpenInBinaryReadMode);

   const pair<size_t, array<char, 256>> fileIsEmptyAndFirst256Bytes = { 0, {} };
   _fileReaderMock->ReadFirst256BytesMock.Return(fileIsEmptyAndFirst256Bytes);

   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const bool isFileEmptyOrBinaryOrNotAnsiOrNotOpenable = _directoryIterator.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(filePath);
   //
   METALMOCKTHEN(_fileOpenerCloserMock->OpenReadModeBinaryFileMock.CalledOnceWith(filePath, false)).Then(
   METALMOCKTHEN(_fileReaderMock->ReadFirst256BytesMock.CalledOnceWith(fileOpenInBinaryReadMode.get())));
   IS_TRUE(isFileEmptyOrBinaryOrNotAnsiOrNotOpenable);
}

TEST(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst256BytesOfFile_FileLengthIsNot0_First256BytesContains0_ReturnsTrue)
{
   const shared_ptr<FILE> fileOpenInBinaryReadMode = make_shared<FILE>();
   _fileOpenerCloserMock->OpenReadModeBinaryFileMock.Return(fileOpenInBinaryReadMode);

   const size_t numberOfBytesRead = ZenUnit::RandomNon0<size_t>();
   const pair<size_t, array<char, 256>> fileIsEmptyAndFirst256Bytes = { numberOfBytesRead, {} };
   _fileReaderMock->ReadFirst256BytesMock.Return(fileIsEmptyAndFirst256Bytes);

   _charArray256HelperMock->ArrayContains0Mock.Return(true);

   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const bool isFileEmptyOrBinaryOrNotAnsiOrNotOpenable = _directoryIterator.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(filePath);
   //
   METALMOCKTHEN(_fileOpenerCloserMock->OpenReadModeBinaryFileMock.CalledOnceWith(filePath, false)).Then(
   METALMOCKTHEN(_fileReaderMock->ReadFirst256BytesMock.CalledOnceWith(fileOpenInBinaryReadMode.get()))).Then(
   METALMOCKTHEN(_charArray256HelperMock->ArrayContains0Mock.CalledOnceWith(fileIsEmptyAndFirst256Bytes.second, numberOfBytesRead)));
   IS_TRUE(isFileEmptyOrBinaryOrNotAnsiOrNotOpenable);
}

TEST(IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable_OpenReadModeBinaryFileReturnsNotNullptr_ReadsFirst256BytesOfFile_FileLengthIsNot0_First256BytesDoesNotContain0_ReturnsFalse)
{
   const shared_ptr<FILE> fileOpenInBinaryReadMode = make_shared<FILE>();
   _fileOpenerCloserMock->OpenReadModeBinaryFileMock.Return(fileOpenInBinaryReadMode);

   const size_t numberOfBytesRead = ZenUnit::RandomNon0<size_t>();
   const pair<size_t, array<char, 256>> fileIsEmptyAndFirst256Bytes = { numberOfBytesRead, {} };
   _fileReaderMock->ReadFirst256BytesMock.Return(fileIsEmptyAndFirst256Bytes);

   _charArray256HelperMock->ArrayContains0Mock.Return(false);

   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const bool isFileEmptyOrBinaryOrNotAnsiOrNotOpenable = _directoryIterator.IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(filePath);
   //
   METALMOCKTHEN(_fileOpenerCloserMock->OpenReadModeBinaryFileMock.CalledOnceWith(filePath, false)).Then(
   METALMOCKTHEN(_fileReaderMock->ReadFirst256BytesMock.CalledOnceWith(fileOpenInBinaryReadMode.get()))).Then(
   METALMOCKTHEN(_charArray256HelperMock->ArrayContains0Mock.CalledOnceWith(fileIsEmptyAndFirst256Bytes.second, numberOfBytesRead)));
   IS_FALSE(isFileEmptyOrBinaryOrNotAnsiOrNotOpenable);
}

TEST3X3(PathContainsAnySubstringCaseInsensitive_ReturnsTrueIfFilePathCaseInsensitiveContainsAnySubstring,
   const fs::path& fileOrFolderPath, const vector<string>& substrings, bool expectedReturnValue,
   "", vector<string>{}, false,
   "a", vector<string>{"a"}, true,
   "A", vector<string>{"a"}, true,
   "a", vector<string>{"b", "a"}, true,
   "abc bb def", vector<string>{"aa", "bb", "cc"}, true,
   "C:\\Directory\\SubdirectoryC\\FileName.ext", vector<string>{"subdirectoryA\\", "subdirectoryB\\", "subdirectoryC\\"}, true,
   "C:\\Directory\\SubdirectoryC", vector<string>{"subdirectoryA\\", "subdirectoryB\\", "subdirectoryC\\"}, false)
{
   const bool pathContainsAnySubstringCaseInsensitive =
      DirectoryIterator::PathContainsAnySubstringCaseInsensitive(fileOrFolderPath, substrings);
   ARE_EQUAL(expectedReturnValue, pathContainsAnySubstringCaseInsensitive);
}

TEST(SetFileAndFolderPathIgnoreSubstrings_SetsFileAndFolderPathIgnoreSubstringsVector)
{
   const vector<string> fileAndFolderPathIgnoreSubstrings = ZenUnit::RandomVector<string>();
   //
   _directoryIterator.SetFileAndFolderPathIgnoreSubstrings(fileAndFolderPathIgnoreSubstrings);
   //
   VECTORS_ARE_EQUAL(fileAndFolderPathIgnoreSubstrings, _directoryIterator._fileAndFolderPathIgnoreSubstrings);
}

RUN_TESTS(DirectoryIteratorTests)
