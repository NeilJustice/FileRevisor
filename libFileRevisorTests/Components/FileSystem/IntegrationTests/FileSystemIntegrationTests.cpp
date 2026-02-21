#include "pch.h"
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/FileSystem/FileSystemExceptions.h"

TESTS(FileSystemIntegrationTests)
AFACT(GetFilePathsInDirectory_RecurseIsFalse_ReturnsTopLevelFilePaths)
AFACT(GetFilePathsInDirectory_RecurseIsTrue_ReturnsFilePathsInAndBelowFolderPath)
AFACT(GetFolderPathsInDirectory_RecurseIsFalse_ReturnsTopLevelFolderPaths)
AFACT(GetFolderPathsInDirectory_RecurseIsTrue_ReturnsFolderPathsInAndBelowFolderPath)
AFACT(GetStringFolderPathsInDirectory_RecurseIsFalse_ReturnsTopLevelFolderPaths)
AFACT(GetStringFolderPathsInDirectory_RecurseIsTrue_ReturnsFolderPathsInAndBelowFolderPath)
AFACT(ReadText_FileDoesNotExist_ThrowsFileSystemException)
AFACT(ReadText_FileExists_FileIsEmpty_ReturnsEmptyString)
AFACT(ReadText_FileExists_FileIsNotEmptyAndContainsTrailingBinaryZeros_ReturnsFileTextMinusTrailingBinaryZeros)
AFACT(RemoveFile_FileExistsAndIsDeletable_IgnoreFileDeleteErrorIsEitherTrueOrFalse_DeletesTheFile)
AFACT(RemoveFile_FileDoesNotExist_IgnoreFileDeleteErrorIsFalse_ThrowsFileSystemException)
AFACT(RemoveFile_FileDoesNotExist_IgnoreFileDeleteErrorIsTrue_DoesNotThrowException)
EVIDENCE

FileSystem p_fileSystem;
const fs::path _rootFolderPath = "FileSystemIntegrationTests";

void CreateIntegrationTestsDirectoryStructure()
{
  FileSystem fileSystem;
  const array<char, 2> bytesContaining0A { 1, 0 };
  const array<char, 4> bytesContaining0B { 1, 2, 0, 3 };
  fileSystem.CreateTextFile(_rootFolderPath / ".git/gitfile1", "");
  fileSystem.CreateTextFile(_rootFolderPath / ".git/gitfile2", "");
  fileSystem.CreateTextFile(_rootFolderPath / ".git/gitfile3", "");
  fileSystem.CreateTextFile(_rootFolderPath / "root.emptyFile1", "");
  fileSystem.CreateTextFile(_rootFolderPath / "root.textFile1", "abc");
  fileSystem.CreateFileWithBytes(_rootFolderPath / "root.binaryFile1", bytesContaining0A.data(), sizeof(bytesContaining0A));
  fileSystem.CreateTextFile(_rootFolderPath / "root.textFile2", "123");
  fileSystem.CreateTextFile(_rootFolderPath / "subdirectory1/subdirectory1.emptyFile1", "");
  fileSystem.CreateTextFile(_rootFolderPath / "subdirectory1/subdirectory1.textFile1", "abc");
  fileSystem.CreateFileWithBytes(_rootFolderPath / "subdirectory1/subdirectory1.binaryFile1", bytesContaining0B.data(), sizeof(bytesContaining0B));
  fileSystem.CreateDirectories(_rootFolderPath / "subdirectory1/subdirectoryA/subdirectoryB");
  fileSystem.CreateTextFile(_rootFolderPath / "subdirectory2/subdirectory2.emptyFile1", "");
  fileSystem.CreateTextFile(_rootFolderPath / "subdirectory2/subdirectory2.textFile1", "123");
  fileSystem.CreateFileWithBytes(_rootFolderPath / "subdirectory2/subdirectory2.binaryFile1", bytesContaining0B.data(), sizeof(bytesContaining0B));
  fileSystem.CreateDirectories(_rootFolderPath / "subdirectory3");
  fileSystem.CreateDirectories(_rootFolderPath / "subdirectory3/subdirectory4");
}

STARTUP
{
  fs::remove_all(_rootFolderPath);
  CreateIntegrationTestsDirectoryStructure();
}

CLEANUP
{
  fs::remove_all(_rootFolderPath);
}

TEST(GetFilePathsInDirectory_RecurseIsFalse_ReturnsTopLevelFilePaths)
{
  const vector<fs::path> topLevelFilePathsInDirectory = p_fileSystem.GetFilePathsInDirectory(_rootFolderPath, false);
  //
  const vector<fs::path> expectedTopLevelFilePathsInDirectory =
  {
     _rootFolderPath / "root.binaryFile1",
     _rootFolderPath / "root.emptyFile1",
     _rootFolderPath / "root.textFile1",
     _rootFolderPath / "root.textFile2"
  };
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedTopLevelFilePathsInDirectory, topLevelFilePathsInDirectory);
}

TEST(GetFilePathsInDirectory_RecurseIsTrue_ReturnsFilePathsInAndBelowFolderPath)
{
  const vector<fs::path> filePathsInAndBelowDirectory = p_fileSystem.GetFilePathsInDirectory(_rootFolderPath, true);
  //
  const vector<fs::path> expectedFilePathsInAndBelowDirectory =
  {
     _rootFolderPath / ".git/gitfile1",
     _rootFolderPath / ".git/gitfile2",
     _rootFolderPath / ".git/gitfile3",
     _rootFolderPath / "root.binaryFile1",
     _rootFolderPath / "root.emptyFile1",
     _rootFolderPath / "root.textFile1",
     _rootFolderPath / "root.textFile2",
     _rootFolderPath / "subdirectory1/subdirectory1.binaryFile1",
     _rootFolderPath / "subdirectory1/subdirectory1.emptyFile1",
     _rootFolderPath / "subdirectory1/subdirectory1.textFile1",
     _rootFolderPath / "subdirectory2/subdirectory2.binaryFile1",
     _rootFolderPath / "subdirectory2/subdirectory2.emptyFile1",
     _rootFolderPath / "subdirectory2/subdirectory2.textFile1"
  };
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedFilePathsInAndBelowDirectory, filePathsInAndBelowDirectory);
}

TEST(GetFolderPathsInDirectory_RecurseIsFalse_ReturnsTopLevelFolderPaths)
{
  const vector<fs::path> topLevelFolderPathsInDirectory = p_fileSystem.GetFolderPathsInDirectory(_rootFolderPath, false);
  //
  const vector<fs::path> expectedTopLevelFolderPathsInDirectory =
  {
     _rootFolderPath / ".git",
     _rootFolderPath / "subdirectory1",
     _rootFolderPath / "subdirectory2",
     _rootFolderPath / "subdirectory3"
  };
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedTopLevelFolderPathsInDirectory, topLevelFolderPathsInDirectory);
}

TEST(GetFolderPathsInDirectory_RecurseIsTrue_ReturnsFolderPathsInAndBelowFolderPath)
{
  const vector<fs::path> directoryPathsInAndBelowDirectory = p_fileSystem.GetFolderPathsInDirectory(_rootFolderPath, true);
  //
  const vector<fs::path> expectedFolderPathsInAndBelowDirectory =
  {
     _rootFolderPath / ".git",
     _rootFolderPath / "subdirectory1",
     _rootFolderPath / "subdirectory1/subdirectoryA",
     _rootFolderPath / "subdirectory1/subdirectoryA/subdirectoryB",
     _rootFolderPath / "subdirectory2",
     _rootFolderPath / "subdirectory3",
     _rootFolderPath / "subdirectory3/subdirectory4"
  };
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedFolderPathsInAndBelowDirectory, directoryPathsInAndBelowDirectory);
}

TEST(GetStringFolderPathsInDirectory_RecurseIsFalse_ReturnsTopLevelFolderPaths)
{
  const vector<string> topLevelFolderPathsInDirectory = p_fileSystem.GetStringFolderPathsInDirectory(_rootFolderPath, false);
  //
  const vector<string> expectedTopLevelFolderPathsInDirectory =
  {
     (_rootFolderPath / ".git").string(),
     (_rootFolderPath / "subdirectory1").string(),
     (_rootFolderPath / "subdirectory2").string(),
     (_rootFolderPath / "subdirectory3").string()
  };
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedTopLevelFolderPathsInDirectory, topLevelFolderPathsInDirectory);
}

TEST(GetStringFolderPathsInDirectory_RecurseIsTrue_ReturnsFolderPathsInAndBelowFolderPath)
{
  const vector<string> directoryPathsInAndBelowDirectory = p_fileSystem.GetStringFolderPathsInDirectory(_rootFolderPath, true);
  //
  const vector<string> expectedFolderPathsInAndBelowDirectory =
  {
     (_rootFolderPath / ".git").string(),
     (_rootFolderPath / "subdirectory1").string(),
     (_rootFolderPath / "subdirectory1" / "subdirectoryA").string(),
     (_rootFolderPath / "subdirectory1" / "subdirectoryA" / "subdirectoryB").string(),
     (_rootFolderPath / "subdirectory2").string(),
     (_rootFolderPath / "subdirectory3").string(),
     (_rootFolderPath / "subdirectory3" / "subdirectory4").string()
  };
  INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedFolderPathsInAndBelowDirectory, directoryPathsInAndBelowDirectory);
}

TEST(ReadText_FileDoesNotExist_ThrowsFileSystemException)
{
  const fs::path textFilePathThatDoesNotExist = ZenUnit::Random<fs::path>();
  FileSystemExceptionMaker fileSystemExceptionThrower;
  const FileSystemException expectedFileSystemException =
     fileSystemExceptionThrower.MakeFileSystemExceptionForFailedToOpenFileWithFStream(textFilePathThatDoesNotExist);
  //
  THROWS_EXCEPTION(const string fileText = p_fileSystem.ReadText(textFilePathThatDoesNotExist),
     FileSystemException, expectedFileSystemException.what());
}

TEST(ReadText_FileExists_FileIsEmpty_ReturnsEmptyString)
{
  const fs::path textFilePath = _rootFolderPath / "ReadTextTest.txt";
  p_fileSystem.CreateTextFile(textFilePath, "");
  //
  const string fileText = p_fileSystem.ReadText(textFilePath);
  //
  IS_EMPTY_STRING(fileText);
}

TEST(ReadText_FileExists_FileIsNotEmptyAndContainsTrailingBinaryZeros_ReturnsFileTextMinusTrailingBinaryZeros)
{
  const fs::path textFilePath = _rootFolderPath / "ReadTextTest.txt";
  const array<char, 6> fileBytes { 'a', 'b', '\n', 'c', 0, 0 };
  p_fileSystem.CreateFileWithBytes(textFilePath, fileBytes.data(), sizeof(fileBytes));
  //
  const string fileText = p_fileSystem.ReadText(textFilePath);
  //
  const string expectedFileText = Utils::String::ConcatStrings("a", "b", "\n", "c");
  ARE_EQUAL(expectedFileText, fileText);
}

TEST(RemoveFile_FileExistsAndIsDeletable_IgnoreFileDeleteErrorIsEitherTrueOrFalse_DeletesTheFile)
{
  const string filePathThatExists = (_rootFolderPath / "DeleteFileTest.txt").string();
  p_fileSystem.CreateTextFile(filePathThatExists, ZenUnit::Random<string>());
  IS_TRUE(p_fileSystem.FileOrDirectoryExists(filePathThatExists));
  const bool skipFilesInUse = ZenUnit::Random<bool>();
  //
  p_fileSystem.RemoveFile(filePathThatExists.c_str(), skipFilesInUse);
  //
  IS_FALSE(p_fileSystem.FileOrDirectoryExists(filePathThatExists));
}

TEST(RemoveFile_FileDoesNotExist_IgnoreFileDeleteErrorIsFalse_ThrowsFileSystemException)
{
  const string filePathThatDoesNotExist = ZenUnit::Random<string>();
  IS_FALSE(p_fileSystem.FileOrDirectoryExists(filePathThatDoesNotExist));
  //
  const string expectedExceptionMessage = Utils::String::ConcatStrings(
     "FailedToDeleteFile: unlink(\"", filePathThatDoesNotExist, "\") failed with errno 2 (No such file or directory)");
  THROWS_EXCEPTION(p_fileSystem.RemoveFile(filePathThatDoesNotExist.c_str(), false),
     FileSystemException, expectedExceptionMessage);
}

TEST(RemoveFile_FileDoesNotExist_IgnoreFileDeleteErrorIsTrue_DoesNotThrowException)
{
  const string filePathThatDoesNotExist = ZenUnit::Random<string>();
  IS_FALSE(p_fileSystem.FileOrDirectoryExists(filePathThatDoesNotExist));
  //
  p_fileSystem.RemoveFile(filePathThatDoesNotExist.c_str(), true);
}

RUN_TESTS(FileSystemIntegrationTests)
