#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisor/UtilityComponents/DataStructures/CharArray64Helper.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileOpenerCloserMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileReaderMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"

TESTS(DirectoryIteratorIntegrationTests)
AFACT(IntegrationTest_DirectoryIterator_RecursiveTrue_NextNonIgnoredDirectoryPathReturnsExpectedDirectoryPaths)
AFACT(IntegrationTest_DirectoryIterator_RecursiveFalse_NextNonIgnoredDirectoryPathReturnsEmptyPath)
AFACT(IntegrationTest_DirectoryIterator_RecursiveTrue_NextNonIgnoredFilePathReturnsExpectedFilePaths)
AFACT(IntegrationTest_DirectoryIterator_RecursiveFalse_NextNonIgnoredFilePathReturnsExpectedFilePaths)
EVIDENCE

DirectoryIterator _directoryIterator;
const fs::path _rootDirectoryPath = "DirectoryIteratorIntegrationTests";

void CreateIntegrationTestsDirectoryStructure()
{
   FileSystem fileSystem;
   fileSystem.CreateTextFile(_rootDirectoryPath / "root.file1", "");
   const string file2Path = (_rootDirectoryPath / "root.file2").string();
   fileSystem.CreateTextFile(file2Path, "");
#if defined __linux__|| defined __APPLE__
   const string symlinkPath = (_rootDirectoryPath / "root.symlink3").string();
   int symlinkResult = symlink(file2Path.c_str(), symlinkPath.c_str());
   release_assert(symlinkResult == 0);
#endif
   fileSystem.CreateTextFile(_rootDirectoryPath / "root.ignored_file_extension_1", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "root.ignored_file_extension_2", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory1" / "subdirectory1.file1", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory1" / "subdirectory1.file2", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory1" / "subdirectory1.ignored_extension1", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory1" / "subdirectory1.ignored_extension2", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory2" / "subdirectory2.file1", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory2" / "subdirectory2.ignored_extension1", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory3" / "subdirectory3.ignored_extension2", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "ignored_directory_nameA" / "fileA.txt", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "ignored_directory_nameB" / "fileB.txt", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory4" / "subdirectory4.file1", "");
   fileSystem.CreateTextFile(_rootDirectoryPath / "subdirectory5" / "subdirectory6" / "subdirectory6.file1", "");
   fileSystem.CreateDirectories(_rootDirectoryPath / "subdirectory7");
   fileSystem.CreateDirectories(_rootDirectoryPath / "subdirectory7" / "subdirectory8");
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

TEST(IntegrationTest_DirectoryIterator_RecursiveTrue_NextNonIgnoredDirectoryPathReturnsExpectedDirectoryPaths)
{
   _directoryIterator.SetDirectoryIterator(_rootDirectoryPath, true);

   const vector<string> filePathIgnoreSubstrings = { "ignored_directory_nameA", "ignored_directory_nameB" };
   _directoryIterator.SetFileAndDirectoryPathIgnoreSubstrings(filePathIgnoreSubstrings);

   const fs::path directoryPath1 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath2 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath3 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath4 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath5 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath6 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath7 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath8 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const vector<fs::path> directoryPaths =
   {
      directoryPath1,
      directoryPath2,
      directoryPath3,
      directoryPath4,
      directoryPath5,
      directoryPath6,
      directoryPath7,
      directoryPath8
   };
   const fs::path directoryPath9 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath10 = _directoryIterator.NextNonIgnoredDirectoryPath();
   //
   const vector<fs::path> expectedDirectoryPaths =
   {
      _rootDirectoryPath / "subdirectory1",
      _rootDirectoryPath / "subdirectory2",
      _rootDirectoryPath / "subdirectory3",
      _rootDirectoryPath / "subdirectory4",
      _rootDirectoryPath / "subdirectory5",
      _rootDirectoryPath / "subdirectory5/subdirectory6",
      _rootDirectoryPath / "subdirectory7",
      _rootDirectoryPath / "subdirectory7" / "subdirectory8"
   };
   INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedDirectoryPaths, directoryPaths);
   ARE_EQUAL(fs::path(), directoryPath9);
   ARE_EQUAL(fs::path(), directoryPath10);
}

TEST(IntegrationTest_DirectoryIterator_RecursiveFalse_NextNonIgnoredDirectoryPathReturnsEmptyPath)
{
   _directoryIterator.SetDirectoryIterator(_rootDirectoryPath, false);

   const vector<string> fileAndDirectoryPathIgnoreSubstrings = { "ignored_directory_nameA", "ignored_directory_nameB" };
   _directoryIterator.SetFileAndDirectoryPathIgnoreSubstrings(fileAndDirectoryPathIgnoreSubstrings);

   const fs::path directoryPath1 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath2 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath3 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath4 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath5 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath6 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath7 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const fs::path directoryPath8 = _directoryIterator.NextNonIgnoredDirectoryPath();
   const vector<fs::path> filePaths =
   {
      directoryPath1,
      directoryPath2,
      directoryPath3,
      directoryPath4,
      directoryPath5,
      directoryPath6
   };
   //
   const vector<fs::path> expectedFilePaths =
   {
      _rootDirectoryPath / "subdirectory1",
      _rootDirectoryPath / "subdirectory2",
      _rootDirectoryPath / "subdirectory3",
      _rootDirectoryPath / "subdirectory4",
      _rootDirectoryPath / "subdirectory5",
      _rootDirectoryPath / "subdirectory7"
   };
   INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedFilePaths, filePaths);
   ARE_EQUAL(fs::path(), directoryPath7);
   ARE_EQUAL(fs::path(), directoryPath8);
}

TEST(IntegrationTest_DirectoryIterator_RecursiveTrue_NextNonIgnoredFilePathReturnsExpectedFilePaths)
{
   _directoryIterator.SetDirectoryIterator(_rootDirectoryPath, true);

   const vector<string> fileAndDirectoryPathIgnoreSubstrings = { "ignored", "ignored_extension1", "ignored_extension2" };
   _directoryIterator.SetFileAndDirectoryPathIgnoreSubstrings(fileAndDirectoryPathIgnoreSubstrings);

   const fs::path textFilePath1 = _directoryIterator.NextNonIgnoredFilePath();
   const fs::path textFilePath2 = _directoryIterator.NextNonIgnoredFilePath();
   const fs::path filePath3 = _directoryIterator.NextNonIgnoredFilePath();
   const fs::path filePath4 = _directoryIterator.NextNonIgnoredFilePath();
   const fs::path filePath5 = _directoryIterator.NextNonIgnoredFilePath();
   const fs::path filePath6 = _directoryIterator.NextNonIgnoredFilePath();
   const fs::path filePath7 = _directoryIterator.NextNonIgnoredFilePath();
   const vector<fs::path> filePaths =
   {
      textFilePath1,
      textFilePath2,
      filePath3,
      filePath4,
      filePath5,
      filePath6,
      filePath7
   };
   const fs::path filePath8 = _directoryIterator.NextNonIgnoredFilePath();
   const fs::path filePath9 = _directoryIterator.NextNonIgnoredFilePath();
   //
   const vector<fs::path> expectedFilePaths =
   {
      _rootDirectoryPath / "root.file1",
      _rootDirectoryPath / "root.file2",
      _rootDirectoryPath / "subdirectory1" / "subdirectory1.file1",
      _rootDirectoryPath / "subdirectory1" / "subdirectory1.file2",
      _rootDirectoryPath / "subdirectory2" / "subdirectory2.file1",
      _rootDirectoryPath / "subdirectory4" / "subdirectory4.file1",
      _rootDirectoryPath / "subdirectory5" / "subdirectory6" / "subdirectory6.file1"
   };
   INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedFilePaths, filePaths);
   ARE_EQUAL(fs::path(), filePath8);
   ARE_EQUAL(fs::path(), filePath9);
}

TEST(IntegrationTest_DirectoryIterator_RecursiveFalse_NextNonIgnoredFilePathReturnsExpectedFilePaths)
{
   _directoryIterator.SetDirectoryIterator(_rootDirectoryPath, false);

   const vector<string> fileAndDirectoryPathIgnoreSubstrings = { "ignored_file_extension_1", "ignored_file_extension_2" };
   _directoryIterator.SetFileAndDirectoryPathIgnoreSubstrings(fileAndDirectoryPathIgnoreSubstrings);

   const fs::path textFilePath1 = _directoryIterator.NextNonIgnoredFilePath();
   const fs::path textFilePath2 = _directoryIterator.NextNonIgnoredFilePath();
   const fs::path textFilePath3 = _directoryIterator.NextNonIgnoredFilePath();
   const fs::path textFilePath4 = _directoryIterator.NextNonIgnoredFilePath();
   const vector<fs::path> nonEmptyTextFilePaths =
   {
      textFilePath1,
      textFilePath2
   };
   //
   const vector<fs::path> expectedNonEmptyTextFilePaths =
   {
      _rootDirectoryPath / "root.file1",
      _rootDirectoryPath / "root.file2"
   };
   INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedNonEmptyTextFilePaths, nonEmptyTextFilePaths);
   ARE_EQUAL(fs::path(), textFilePath3);
   ARE_EQUAL(fs::path(), textFilePath4);
}

RUN_TESTS(DirectoryIteratorIntegrationTests)
