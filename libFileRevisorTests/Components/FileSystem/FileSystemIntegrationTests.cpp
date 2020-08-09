#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"
#include "libFileRevisor/Exceptions/FileSystemException.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/ZenMock/NonVoidOneArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/Strings/ZenMock/ConstCharPointerGetterMock.h"

TESTS(FileSystemIntegrationTests)
AFACT(GetFilePathsInDirectory_RecursiveFalse_ReturnsTopLevelFilePaths)
AFACT(GetFilePathsInDirectory_RecursiveTrue_ReturnsFilePathsInAndBelowDirectoryPath)
AFACT(GetDirectoryPathsInDirectory_RecursiveFalse_ReturnsTopLevelDirectoryPaths)
AFACT(GetDirectoryPathsInDirectory_RecursiveTrue_ReturnsDirectoryPathsInAndBelowDirectoryPath)
AFACT(GetStringDirectoryPathsInDirectory_RecursiveFalse_ReturnsTopLevelDirectoryPaths)
AFACT(GetStringDirectoryPathsInDirectory_RecursiveTrue_ReturnsDirectoryPathsInAndBelowDirectoryPath)
AFACT(GetNonEmptyTextFilePathsInDirectory_RecursiveFalse_ReturnsTopLevelFilePathsThatDoNotHaveABinary0InTheFirst1024Bytes)
AFACT(GetNonEmptyTextFilePathsInDirectory_RecursiveTrue_ReturnsAllFilePathsInAndBelowDirectoryThatDoNotHaveABinary0InTheFirst1024Bytes)
AFACT(ReadText_FileDoesNotExist_ThrowsFileSystemException)
AFACT(ReadText_FileExists_FileIsEmpty_ReturnsEmptyString)
AFACT(ReadText_FileExists_FileIsNotEmptyAndContainsTrailingBinaryZeros_ReturnsFileTextMinusTrailingBinaryZeros)
AFACT(RemoveFile_FileExistsAndIsDeletable_DeletesTheFile)
AFACT(RemoveFile_FileDoesNotExist_ThrowsFileSystemException)
AFACT(RemoveAll_DirectoryPathIsEmpty_DoesNothing)
AFACT(RemoveAll_DirectoryPathIsNonEmptyAndDoesNotExist_DoesNothing)
AFACT(RemoveAll_DirectoryExistsAndIsDeletable_DeletesTheDirectory)
EVIDENCE

FileSystem _fileSystem;
const fs::path _rootDirectoryPath = "FileSystemIntegrationTests";

void CreateIntegrationTestsDirectoryStructure()
{
   FileSystem fileSystem;
   const char bytesContaining0A[] = { 1, 0 };
   const char bytesContaining0B[] = { 1, 2, 0, 3 };
   fileSystem.CreateTextFile(_rootDirectoryPath / "root.emptyFile1", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "root.textFile1", "abc");
   fileSystem.CreateBinaryFile(_rootDirectoryPath / "root.binaryFile1", bytesContaining0A, sizeof(bytesContaining0A));
   fileSystem.CreateTextFile(_rootDirectoryPath / "root.textFile2", "123");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory1/subdirectory1.emptyFile1", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory1/subdirectory1.textFile1", "abc");
   fileSystem.CreateBinaryFile(_rootDirectoryPath / "subdirectory1/subdirectory1.binaryFile1", bytesContaining0B, sizeof(bytesContaining0B));
   fileSystem.CreateDirectories(_rootDirectoryPath / "subdirectory1/subdirectoryA/subdirectoryB");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory2/subdirectory2.emptyFile1", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory2/subdirectory2.textFile1", "123");
   fileSystem.CreateBinaryFile(_rootDirectoryPath / "subdirectory2/subdirectory2.binaryFile1", bytesContaining0B, sizeof(bytesContaining0B));
   fileSystem.CreateDirectories(_rootDirectoryPath / "subdirectory3");
   fileSystem.CreateDirectories(_rootDirectoryPath / "subdirectory3/subdirectory4");
}

STARTUP
{
   fs::remove_all(_rootDirectoryPath);
   CreateIntegrationTestsDirectoryStructure();
}

CLEANUP
{
   fs::remove_all(_rootDirectoryPath);
}

TEST(GetFilePathsInDirectory_RecursiveFalse_ReturnsTopLevelFilePaths)
{
   const vector<fs::path> topLevelFilePathsInDirectory =
      _fileSystem.GetFilePathsInDirectory(_rootDirectoryPath, false);
   //
   const vector<fs::path> expectedTopLevelFilePathsInDirectory =
   {
      _rootDirectoryPath / "root.binaryFile1",
      _rootDirectoryPath / "root.emptyFile1",
      _rootDirectoryPath / "root.textFile1",
      _rootDirectoryPath / "root.textFile2"
   };
   EQUAL_ELEMENTS_ANY_ORDER(expectedTopLevelFilePathsInDirectory, topLevelFilePathsInDirectory);
}

TEST(GetFilePathsInDirectory_RecursiveTrue_ReturnsFilePathsInAndBelowDirectoryPath)
{
   const vector<fs::path> filePathsInAndBelowDirectory = _fileSystem.GetFilePathsInDirectory(_rootDirectoryPath, true);
   //
   const vector<fs::path> expectedFilePathsInAndBelowDirectory =
   {
      _rootDirectoryPath / "root.binaryFile1",
      _rootDirectoryPath / "root.emptyFile1",
      _rootDirectoryPath / "root.textFile1",
      _rootDirectoryPath / "root.textFile2",
      _rootDirectoryPath / "subdirectory1/subdirectory1.binaryFile1",
      _rootDirectoryPath / "subdirectory1/subdirectory1.emptyFile1",
      _rootDirectoryPath / "subdirectory1/subdirectory1.textFile1",
      _rootDirectoryPath / "subdirectory2/subdirectory2.binaryFile1",
      _rootDirectoryPath / "subdirectory2/subdirectory2.emptyFile1",
      _rootDirectoryPath / "subdirectory2/subdirectory2.textFile1"
   };
   EQUAL_ELEMENTS_ANY_ORDER(expectedFilePathsInAndBelowDirectory, filePathsInAndBelowDirectory);
}

TEST(GetDirectoryPathsInDirectory_RecursiveFalse_ReturnsTopLevelDirectoryPaths)
{
   const vector<fs::path> topLevelDirectoryPathsInDirectory = _fileSystem.GetDirectoryPathsInDirectory(_rootDirectoryPath, false);
   //
   const vector<fs::path> expectedTopLevelDirectoryPathsInDirectory =
   {
      _rootDirectoryPath / "subdirectory1",
      _rootDirectoryPath / "subdirectory2",
      _rootDirectoryPath / "subdirectory3"
   };
   EQUAL_ELEMENTS_ANY_ORDER(expectedTopLevelDirectoryPathsInDirectory, topLevelDirectoryPathsInDirectory);
}

TEST(GetDirectoryPathsInDirectory_RecursiveTrue_ReturnsDirectoryPathsInAndBelowDirectoryPath)
{
   const vector<fs::path> directoryPathsInAndBelowDirectory = _fileSystem.GetDirectoryPathsInDirectory(_rootDirectoryPath, true);
   //
   const vector<fs::path> expectedDirectoryPathsInAndBelowDirectory =
   {
      _rootDirectoryPath / "subdirectory1",
      _rootDirectoryPath / "subdirectory1/subdirectoryA",
      _rootDirectoryPath / "subdirectory1/subdirectoryA/subdirectoryB",
      _rootDirectoryPath / "subdirectory2",
      _rootDirectoryPath / "subdirectory3",
      _rootDirectoryPath / "subdirectory3/subdirectory4"
   };
   EQUAL_ELEMENTS_ANY_ORDER(expectedDirectoryPathsInAndBelowDirectory, directoryPathsInAndBelowDirectory);
}

TEST(GetStringDirectoryPathsInDirectory_RecursiveFalse_ReturnsTopLevelDirectoryPaths)
{
   const vector<string> topLevelDirectoryPathsInDirectory =
      _fileSystem.GetStringDirectoryPathsInDirectory(_rootDirectoryPath, false);
   //
   const vector<string> expectedTopLevelDirectoryPathsInDirectory =
   {
      (_rootDirectoryPath / "subdirectory1").string(),
      (_rootDirectoryPath / "subdirectory2").string(),
      (_rootDirectoryPath / "subdirectory3").string()
   };
   EQUAL_ELEMENTS_ANY_ORDER(expectedTopLevelDirectoryPathsInDirectory, topLevelDirectoryPathsInDirectory);
}

TEST(GetStringDirectoryPathsInDirectory_RecursiveTrue_ReturnsDirectoryPathsInAndBelowDirectoryPath)
{
   const vector<string> directoryPathsInAndBelowDirectory =
      _fileSystem.GetStringDirectoryPathsInDirectory(_rootDirectoryPath, true);
   //
   const vector<string> expectedDirectoryPathsInAndBelowDirectory =
   {
      (_rootDirectoryPath / "subdirectory1").string(),
      (_rootDirectoryPath / "subdirectory1" / "subdirectoryA").string(),
      (_rootDirectoryPath / "subdirectory1" / "subdirectoryA" / "subdirectoryB").string(),
      (_rootDirectoryPath / "subdirectory2").string(),
      (_rootDirectoryPath / "subdirectory3").string(),
      (_rootDirectoryPath / "subdirectory3" / "subdirectory4").string()
   };
   EQUAL_ELEMENTS_ANY_ORDER(expectedDirectoryPathsInAndBelowDirectory, directoryPathsInAndBelowDirectory);
}

TEST(GetNonEmptyTextFilePathsInDirectory_RecursiveFalse_ReturnsTopLevelFilePathsThatDoNotHaveABinary0InTheFirst1024Bytes)
{
   const vector<fs::path> topLevelNonEmptyTextFilesInDirectory =
      _fileSystem.GetNonEmptyTextFilePathsInDirectory(_rootDirectoryPath, false);
   //
   const vector<fs::path> expectedTopLevelNonEmptyTextFilesInDirectory =
   {
      _rootDirectoryPath / "root.textFile1",
      _rootDirectoryPath / "root.textFile2"
   };
   EQUAL_ELEMENTS_ANY_ORDER(expectedTopLevelNonEmptyTextFilesInDirectory, topLevelNonEmptyTextFilesInDirectory);
}

TEST(GetNonEmptyTextFilePathsInDirectory_RecursiveTrue_ReturnsAllFilePathsInAndBelowDirectoryThatDoNotHaveABinary0InTheFirst1024Bytes)
{
   const vector<fs::path> allNonEmptyTextFilePathsInAndBelowDirectory =
      _fileSystem.GetNonEmptyTextFilePathsInDirectory(_rootDirectoryPath, true);
   //
   const vector<fs::path> expectedAllNonEmptyTextFilePathsInAndBelowDirectory =
   {
      _rootDirectoryPath / "root.textFile1",
      _rootDirectoryPath / "root.textFile2",
      _rootDirectoryPath / "subdirectory1/subdirectory1.textFile1",
      _rootDirectoryPath / "subdirectory2/subdirectory2.textFile1",
   };
   EQUAL_ELEMENTS_ANY_ORDER(
      expectedAllNonEmptyTextFilePathsInAndBelowDirectory,
      allNonEmptyTextFilePathsInAndBelowDirectory);
}

TEST(ReadText_FileDoesNotExist_ThrowsFileSystemException)
{
   const fs::path textFilePathThatDoesNotExist = ZenUnit::Random<fs::path>();

   FileSystemExceptionMaker fileSystemExceptionThrower;
   const FileSystemException expectedFileSystemException = fileSystemExceptionThrower.
      MakeFileSystemExceptionForFailedToOpenFileWithFStream(textFilePathThatDoesNotExist);
   //
   THROWS_EXCEPTION(const string fileText = _fileSystem.ReadText(textFilePathThatDoesNotExist),
      FileSystemException, expectedFileSystemException.what());
}

TEST(ReadText_FileExists_FileIsEmpty_ReturnsEmptyString)
{
   const fs::path textFilePath = _rootDirectoryPath / "ReadTextTest.txt";
   _fileSystem.CreateTextFile(textFilePath, "");
   //
   const string fileText = _fileSystem.ReadText(textFilePath);
   //
   ARE_EQUAL("", fileText);
}

TEST(ReadText_FileExists_FileIsNotEmptyAndContainsTrailingBinaryZeros_ReturnsFileTextMinusTrailingBinaryZeros)
{
   const fs::path textFilePath = _rootDirectoryPath / "ReadTextTest.txt";
   const char fileBytes[] = { 'a', 'b', '\n', 'c', 0, 0 };
   _fileSystem.CreateBinaryFile(textFilePath, fileBytes, sizeof(fileBytes));
   //
   const string fileText = _fileSystem.ReadText(textFilePath);
   //
   const string expectedFileText = String::Concat('a', 'b', '\n', 'c');
   ARE_EQUAL(expectedFileText, fileText);
}

TEST(RemoveFile_FileExistsAndIsDeletable_DeletesTheFile)
{
   const string filePathThatExists = (_rootDirectoryPath / "RemoveFileTest.txt").string();
   _fileSystem.CreateTextFile(filePathThatExists, ZenUnit::Random<string>());
   IS_TRUE(_fileSystem.FileOrDirectoryExists(filePathThatExists));
   //
   _fileSystem.RemoveFile(filePathThatExists.c_str());
   //
   IS_FALSE(_fileSystem.FileOrDirectoryExists(filePathThatExists));
}

TEST(RemoveFile_FileDoesNotExist_ThrowsFileSystemException)
{
   const string filePathThatDoesNotExist = ZenUnit::Random<string>();
   IS_FALSE(_fileSystem.FileOrDirectoryExists(filePathThatDoesNotExist));
   //
   const string expectedExceptionMessage = "FailedToDeleteFile: unlink(\"" + filePathThatDoesNotExist +
      "\") failed with errno = 2 (No such file or directory)";
   THROWS_EXCEPTION(_fileSystem.RemoveFile(filePathThatDoesNotExist.c_str()),
      FileSystemException, expectedExceptionMessage);
}

TEST(RemoveAll_DirectoryPathIsEmpty_DoesNothing)
{
   const fs::path emptyDirectoryPath;
   IS_FALSE(_fileSystem.FileOrDirectoryExists(emptyDirectoryPath));
   //
   _fileSystem.RemoveAll(emptyDirectoryPath);
   //
   IS_FALSE(_fileSystem.FileOrDirectoryExists(emptyDirectoryPath));
}

TEST(RemoveAll_DirectoryPathIsNonEmptyAndDoesNotExist_DoesNothing)
{
   const fs::path directoryPathThatDoesNotExist(_rootDirectoryPath / ZenUnit::Random<fs::path>());
   IS_FALSE(_fileSystem.FileOrDirectoryExists(directoryPathThatDoesNotExist));
   //
   _fileSystem.RemoveAll(directoryPathThatDoesNotExist);
   //
   IS_FALSE(_fileSystem.FileOrDirectoryExists(directoryPathThatDoesNotExist));
}

TEST(RemoveAll_DirectoryExistsAndIsDeletable_DeletesTheDirectory)
{
   IS_TRUE(_fileSystem.FileOrDirectoryExists(_rootDirectoryPath));
   //
   _fileSystem.RemoveAll(_rootDirectoryPath);
   //
   IS_FALSE(_fileSystem.FileOrDirectoryExists(_rootDirectoryPath));
}

RUN_TESTS(FileSystemIntegrationTests)
