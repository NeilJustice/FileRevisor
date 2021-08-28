#include "pch.h"
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Exceptions/FileSystemException.h"
#include "libFileRevisorTests/UtilityComponents/FunctionCallers/Member/MetalMock/NonVoidOneArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/UtilityComponents/Strings/MetalMock/ConstCharPointerGetterMock.h"

TESTS(FileSystemIntegrationTests)
AFACT(GetFilePathsInDirectory_RecurseIsFalse_ReturnsTopLevelFilePaths)
AFACT(GetFilePathsInDirectory_RecurseIsTrue_ReturnsFilePathsInAndBelowDirectoryPath)
AFACT(GetDirectoryPathsInDirectory_RecurseIsFalse_ReturnsTopLevelDirectoryPaths)
AFACT(GetDirectoryPathsInDirectory_RecurseIsTrue_ReturnsDirectoryPathsInAndBelowDirectoryPath)
AFACT(GetStringDirectoryPathsInDirectory_RecurseIsFalse_ReturnsTopLevelDirectoryPaths)
AFACT(GetStringDirectoryPathsInDirectory_RecurseIsTrue_ReturnsDirectoryPathsInAndBelowDirectoryPath)
AFACT(ReadText_FileDoesNotExist_ThrowsFileSystemException)
AFACT(ReadText_FileExists_FileIsEmpty_ReturnsEmptyString)
AFACT(ReadText_FileExists_FileIsNotEmptyAndContainsTrailingBinaryZeros_ReturnsFileTextMinusTrailingBinaryZeros)
AFACT(RemoveFile_FileExistsAndIsDeletable_IgnoreFileDeleteErrorIsEitherTrueOrFalse_DeletesTheFile)
AFACT(RemoveFile_FileDoesNotExist_IgnoreFileDeleteErrorIsFalse_ThrowsFileSystemException)
AFACT(RemoveFile_FileDoesNotExist_IgnoreFileDeleteErrorIsTrue_DoesNotThrowException)
EVIDENCE

FileSystem _fileSystem;
const fs::path _rootDirectoryPath = "FileSystemIntegrationTests";

void CreateIntegrationTestsDirectoryStructure()
{
  FileSystem fileSystem;
  const char bytesContaining0A[] = { 1, 0 };
  const char bytesContaining0B[] = { 1, 2, 0, 3 };
  fileSystem.CreateTextFile(_rootDirectoryPath / ".git/gitfile1", "");
  fileSystem.CreateTextFile(_rootDirectoryPath / ".git/gitfile2", "");
  fileSystem.CreateTextFile(_rootDirectoryPath / ".git/gitfile3", "");
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

TEST(GetFilePathsInDirectory_RecurseIsFalse_ReturnsTopLevelFilePaths)
{
  const vector<fs::path> topLevelFilePathsInDirectory = _fileSystem.GetFilePathsInDirectory(_rootDirectoryPath, false);
  //
  const vector<fs::path> expectedTopLevelFilePathsInDirectory =
  {
     _rootDirectoryPath / "root.binaryFile1",
     _rootDirectoryPath / "root.emptyFile1",
     _rootDirectoryPath / "root.textFile1",
     _rootDirectoryPath / "root.textFile2"
  };
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedTopLevelFilePathsInDirectory, topLevelFilePathsInDirectory);
}

TEST(GetFilePathsInDirectory_RecurseIsTrue_ReturnsFilePathsInAndBelowDirectoryPath)
{
  const vector<fs::path> filePathsInAndBelowDirectory = _fileSystem.GetFilePathsInDirectory(_rootDirectoryPath, true);
  //
  const vector<fs::path> expectedFilePathsInAndBelowDirectory =
  {
     _rootDirectoryPath / ".git/gitfile1",
     _rootDirectoryPath / ".git/gitfile2",
     _rootDirectoryPath / ".git/gitfile3",
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
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedFilePathsInAndBelowDirectory, filePathsInAndBelowDirectory);
}

TEST(GetDirectoryPathsInDirectory_RecurseIsFalse_ReturnsTopLevelDirectoryPaths)
{
  const vector<fs::path> topLevelDirectoryPathsInDirectory = _fileSystem.GetDirectoryPathsInDirectory(_rootDirectoryPath, false);
  //
  const vector<fs::path> expectedTopLevelDirectoryPathsInDirectory =
  {
     _rootDirectoryPath / ".git",
     _rootDirectoryPath / "subdirectory1",
     _rootDirectoryPath / "subdirectory2",
     _rootDirectoryPath / "subdirectory3"
  };
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedTopLevelDirectoryPathsInDirectory, topLevelDirectoryPathsInDirectory);
}

TEST(GetDirectoryPathsInDirectory_RecurseIsTrue_ReturnsDirectoryPathsInAndBelowDirectoryPath)
{
  const vector<fs::path> directoryPathsInAndBelowDirectory = _fileSystem.GetDirectoryPathsInDirectory(_rootDirectoryPath, true);
  //
  const vector<fs::path> expectedDirectoryPathsInAndBelowDirectory =
  {
     _rootDirectoryPath / ".git",
     _rootDirectoryPath / "subdirectory1",
     _rootDirectoryPath / "subdirectory1/subdirectoryA",
     _rootDirectoryPath / "subdirectory1/subdirectoryA/subdirectoryB",
     _rootDirectoryPath / "subdirectory2",
     _rootDirectoryPath / "subdirectory3",
     _rootDirectoryPath / "subdirectory3/subdirectory4"
  };
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedDirectoryPathsInAndBelowDirectory, directoryPathsInAndBelowDirectory);
}

TEST(GetStringDirectoryPathsInDirectory_RecurseIsFalse_ReturnsTopLevelDirectoryPaths)
{
  const vector<string> topLevelDirectoryPathsInDirectory = _fileSystem.GetStringDirectoryPathsInDirectory(_rootDirectoryPath, false);
  //
  const vector<string> expectedTopLevelDirectoryPathsInDirectory =
  {
     (_rootDirectoryPath / ".git").string(),
     (_rootDirectoryPath / "subdirectory1").string(),
     (_rootDirectoryPath / "subdirectory2").string(),
     (_rootDirectoryPath / "subdirectory3").string()
  };
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedTopLevelDirectoryPathsInDirectory, topLevelDirectoryPathsInDirectory);
}

TEST(GetStringDirectoryPathsInDirectory_RecurseIsTrue_ReturnsDirectoryPathsInAndBelowDirectoryPath)
{
  const vector<string> directoryPathsInAndBelowDirectory = _fileSystem.GetStringDirectoryPathsInDirectory(_rootDirectoryPath, true);
  //
  const vector<string> expectedDirectoryPathsInAndBelowDirectory =
  {
     (_rootDirectoryPath / ".git").string(),
     (_rootDirectoryPath / "subdirectory1").string(),
     (_rootDirectoryPath / "subdirectory1" / "subdirectoryA").string(),
     (_rootDirectoryPath / "subdirectory1" / "subdirectoryA" / "subdirectoryB").string(),
     (_rootDirectoryPath / "subdirectory2").string(),
     (_rootDirectoryPath / "subdirectory3").string(),
     (_rootDirectoryPath / "subdirectory3" / "subdirectory4").string()
  };
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedDirectoryPathsInAndBelowDirectory, directoryPathsInAndBelowDirectory);
}

TEST(ReadText_FileDoesNotExist_ThrowsFileSystemException)
{
  const fs::path textFilePathThatDoesNotExist = ZenUnit::Random<fs::path>();
  FileSystemExceptionMaker fileSystemExceptionThrower;
  const FileSystemException expectedFileSystemException =
     fileSystemExceptionThrower.MakeFileSystemExceptionForFailedToOpenFileWithFStream(textFilePathThatDoesNotExist);
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
  IS_EMPTY_STRING(fileText);
}

TEST(ReadText_FileExists_FileIsNotEmptyAndContainsTrailingBinaryZeros_ReturnsFileTextMinusTrailingBinaryZeros)
{
  const fs::path textFilePath = _rootDirectoryPath / "ReadTextTest.txt";
  const char fileBytes[] = { 'a', 'b', '\n', 'c', 0, 0 };
  _fileSystem.CreateBinaryFile(textFilePath, fileBytes, sizeof(fileBytes));
  //
  const string fileText = _fileSystem.ReadText(textFilePath);
  //
  const string expectedFileText = String::ConcatStrings("a", "b", "\n", "c");
  ARE_EQUAL(expectedFileText, fileText);
}

TEST(RemoveFile_FileExistsAndIsDeletable_IgnoreFileDeleteErrorIsEitherTrueOrFalse_DeletesTheFile)
{
  const string filePathThatExists = (_rootDirectoryPath / "DeleteFileTest.txt").string();
  _fileSystem.CreateTextFile(filePathThatExists, ZenUnit::Random<string>());
  IS_TRUE(_fileSystem.FileOrDirectoryExists(filePathThatExists));
  const bool skipFilesInUse = ZenUnit::Random<bool>();
  //
  _fileSystem.RemoveFile(filePathThatExists.c_str(), skipFilesInUse);
  //
  IS_FALSE(_fileSystem.FileOrDirectoryExists(filePathThatExists));
}

TEST(RemoveFile_FileDoesNotExist_IgnoreFileDeleteErrorIsFalse_ThrowsFileSystemException)
{
  const string filePathThatDoesNotExist = ZenUnit::Random<string>();
  IS_FALSE(_fileSystem.FileOrDirectoryExists(filePathThatDoesNotExist));
  //
  const string expectedExceptionMessage = String::ConcatStrings(
     "FailedToDeleteFile: unlink(\"", filePathThatDoesNotExist, "\") failed with errno 2 (No such file or directory)");
  THROWS_EXCEPTION(_fileSystem.RemoveFile(filePathThatDoesNotExist.c_str(), false),
     FileSystemException, expectedExceptionMessage);
}

TEST(RemoveFile_FileDoesNotExist_IgnoreFileDeleteErrorIsTrue_DoesNotThrowException)
{
  const string filePathThatDoesNotExist = ZenUnit::Random<string>();
  IS_FALSE(_fileSystem.FileOrDirectoryExists(filePathThatDoesNotExist));
  //
  _fileSystem.RemoveFile(filePathThatDoesNotExist.c_str(), true);
}

RUN_TESTS(FileSystemIntegrationTests)
