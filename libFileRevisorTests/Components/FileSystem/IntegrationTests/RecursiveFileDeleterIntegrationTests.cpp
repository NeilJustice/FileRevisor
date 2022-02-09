#include "pch.h"
#include "libFileRevisor/Components/ErrorHandling/ErrorCodeTranslator.h"
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"
#include "libFileRevisor/Components/FileSystem/RecursiveFileDeleter.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#if defined __linux__|| defined __APPLE__
#include <sys/stat.h>
#endif

TESTS(RecursiveFileDeleterIntegrationTests)
AFACT(RecursivelyDeleteAllFilesInDirectory_DryRunIsTrue_RecursivelyPrintsWouldDeleteFile)
AFACT(RecursivelyDeleteAllFilesInDirectory_DryRunIsFalse_RecursivelyPrintsWouldDeleteFile)
EVIDENCE

RecursiveFileDeleter _recursiveFileDeleter;
FileSystem _fileSystem;

string _rootDirectoryPathString;
fs::path _rootDirectoryPath;
fs::path _root_file1;
fs::path _root_file2;
fs::path _root_subdirectory1;
fs::path _root_subdirectory1_file1;
fs::path _root_subdirectory1_file2;
fs::path _root_subdirectory2;
fs::path _root_subdirectory2_subdirectory3;
fs::path _root_subdirectory2_subdirectory3_file1;
fs::path _root_subdirectory2_subdirectory3_file2;
fs::path _root_subdirectory4;
fs::path _root_subdirectory5;
fs::path _root_subdirectory5_file1;

STARTUP
{
  _rootDirectoryPathString = "RecursiveFileDeleterIntegrationTests";
  _rootDirectoryPath = _rootDirectoryPathString;

  fs::remove_all(_rootDirectoryPath);

  _fileSystem.CreateFileWithText(_root_file1 = _rootDirectoryPath / "file1.ext", ZenUnit::Random<string>());
  _fileSystem.CreateFileWithText(_root_file2 = _rootDirectoryPath / ".gitignore", ZenUnit::Random<string>());

  _fileSystem.CreateDirectories(_root_subdirectory1 = _rootDirectoryPath / "subdirectory1");
  _fileSystem.CreateFileWithText(_root_subdirectory1_file1 = _rootDirectoryPath / "subdirectory1" / "file1.ext", ZenUnit::Random<string>());
  _fileSystem.CreateFileWithText(_root_subdirectory1_file2 = _rootDirectoryPath / "subdirectory1" / "file2.ext", ZenUnit::Random<string>());

  _fileSystem.CreateDirectories(_root_subdirectory2 = _rootDirectoryPath / "subdirectory2");
  _fileSystem.CreateDirectories(_root_subdirectory2_subdirectory3 = _rootDirectoryPath / "subdirectory2" / "subdirectory3");
  _fileSystem.CreateFileWithText(_root_subdirectory2_subdirectory3_file1 = _rootDirectoryPath / "subdirectory2" / "subdirectory3" / "file1.ext", ZenUnit::Random<string>());
  _fileSystem.CreateFileWithText(_root_subdirectory2_subdirectory3_file2 = _rootDirectoryPath / "subdirectory2" / "subdirectory3" / "file2.ext", ZenUnit::Random<string>());

  _fileSystem.CreateDirectories(_root_subdirectory4 = _rootDirectoryPath / "subdirectory4");

  _fileSystem.CreateDirectories(_root_subdirectory5 = _rootDirectoryPath / "subdirectory5");
  _fileSystem.CreateFileWithText(_root_subdirectory5_file1 = _rootDirectoryPath / "subdirectory5" / "file1.ext", ZenUnit::Random<string>());
}

CLEANUP
{
#if defined __linux__|| defined __APPLE__
  Chmod777Directory(_root_subdirectory5);
#endif
   fs::remove_all(_rootDirectoryPath);
}

void AssertExpectedStartingStateOfFileSystem() const
{
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_rootDirectoryPath));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_file1));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_file2));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory1));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory1_file1));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory1_file2));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory2));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory2_subdirectory3));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory2_subdirectory3_file1));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory2_subdirectory3_file2));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory4));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory5));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory5_file1));
}

void AssertExpectedEndingStateOfFileSystem() const
{
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_rootDirectoryPath));
  IS_FALSE(_fileSystem.FileOrDirectoryExists(_root_file1));
  IS_FALSE(_fileSystem.FileOrDirectoryExists(_root_file2));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory1));
  IS_FALSE(_fileSystem.FileOrDirectoryExists(_root_subdirectory1_file1));
  IS_FALSE(_fileSystem.FileOrDirectoryExists(_root_subdirectory1_file2));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory2));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory2_subdirectory3));
  IS_FALSE(_fileSystem.FileOrDirectoryExists(_root_subdirectory2_subdirectory3_file1));
  IS_FALSE(_fileSystem.FileOrDirectoryExists(_root_subdirectory2_subdirectory3_file2));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory4));
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_root_subdirectory5));
  IS_FALSE(_fileSystem.FileOrDirectoryExists(_root_subdirectory5_file1));
}

TEST(RecursivelyDeleteAllFilesInDirectory_DryRunIsTrue_RecursivelyPrintsWouldDeleteFile)
{
  AssertExpectedStartingStateOfFileSystem();
  FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
  args.dryrun = true;
  //
  _recursiveFileDeleter.RecursivelyDeleteAllFilesInDirectory(_rootDirectoryPathString.c_str(), args);
  //
  AssertExpectedStartingStateOfFileSystem();
}

TEST(RecursivelyDeleteAllFilesInDirectory_DryRunIsFalse_RecursivelyPrintsWouldDeleteFile)
{
  AssertExpectedStartingStateOfFileSystem();
  FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
  args.dryrun = false;
  //
  _recursiveFileDeleter.RecursivelyDeleteAllFilesInDirectory(_rootDirectoryPathString.c_str(), args);
  //
  AssertExpectedEndingStateOfFileSystem();
}

#if defined __linux__|| defined __APPLE__
// chmod 555 = "dr-xr-xr-x"
static void Chmod555Directory(const fs::path& directoryPath)
{
  const int userRead_groupRead_othersRead = S_IRUSR | S_IRGRP | S_IROTH;
  const int chmodReturnValue = chmod(directoryPath.string().c_str(), userRead_groupRead_othersRead);
  release_assert(chmodReturnValue == 0);
}

// chmod 777 = "drwxrwxrwx"
static void Chmod777Directory(const fs::path& directoryPath)
{
  const int chmodReturnValue = chmod(directoryPath.string().c_str(), 0777);
  release_assert(chmodReturnValue == 0);
}
#endif

RUN_TESTS(RecursiveFileDeleterIntegrationTests)
