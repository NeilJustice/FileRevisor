#include "pch.h"
#include "libFileRevisor/Components/FileSystem/RecursiveFileDeleter.h"
#if defined __linux__
#include <sys/stat.h>
#endif

TESTS(RecursiveFileDeleterIntegrationTests)
AFACT(RecursivelyDeleteAllFilesInDirectory_DryRunIsTrue_RecursivelyPrintsWouldDeleteFile)
AFACT(RecursivelyDeleteAllFilesInDirectory_DryRunIsFalse_RecursivelyPrintsWouldDeleteFile)
EVIDENCE

RecursiveFileDeleter _recursiveFileDeleter;
FileSystem _fileSystem;

string _rootFolderPathString;
fs::path _rootFolderPath;
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
  _rootFolderPathString = "RecursiveFileDeleterIntegrationTests";
  _rootFolderPath = _rootFolderPathString;

  fs::remove_all(_rootFolderPath);

  _fileSystem.CreateTextFile(_root_file1 = _rootFolderPath / "file1.ext", ZenUnit::Random<string>());
  _fileSystem.CreateTextFile(_root_file2 = _rootFolderPath / ".gitignore", ZenUnit::Random<string>());

  _fileSystem.CreateDirectories(_root_subdirectory1 = _rootFolderPath / "subdirectory1");
  _fileSystem.CreateTextFile(_root_subdirectory1_file1 = _rootFolderPath / "subdirectory1" / "file1.ext", ZenUnit::Random<string>());
  _fileSystem.CreateTextFile(_root_subdirectory1_file2 = _rootFolderPath / "subdirectory1" / "file2.ext", ZenUnit::Random<string>());

  _fileSystem.CreateDirectories(_root_subdirectory2 = _rootFolderPath / "subdirectory2");
  _fileSystem.CreateDirectories(_root_subdirectory2_subdirectory3 = _rootFolderPath / "subdirectory2" / "subdirectory3");
  _fileSystem.CreateTextFile(_root_subdirectory2_subdirectory3_file1 = _rootFolderPath / "subdirectory2" / "subdirectory3" / "file1.ext", ZenUnit::Random<string>());
  _fileSystem.CreateTextFile(_root_subdirectory2_subdirectory3_file2 = _rootFolderPath / "subdirectory2" / "subdirectory3" / "file2.ext", ZenUnit::Random<string>());

  _fileSystem.CreateDirectories(_root_subdirectory4 = _rootFolderPath / "subdirectory4");

  _fileSystem.CreateDirectories(_root_subdirectory5 = _rootFolderPath / "subdirectory5");
  _fileSystem.CreateTextFile(_root_subdirectory5_file1 = _rootFolderPath / "subdirectory5" / "file1.ext", ZenUnit::Random<string>());
}

CLEANUP
{
#if defined __linux__
  Chmod777Directory(_root_subdirectory5);
#endif
   fs::remove_all(_rootFolderPath);
}

void AssertExpectedStartingStateOfFileSystem() const
{
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_rootFolderPath));
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
  IS_TRUE(_fileSystem.FileOrDirectoryExists(_rootFolderPath));
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
  _recursiveFileDeleter.RecursivelyDeleteAllFilesInDirectory(_rootFolderPathString.c_str(), args);
  //
  AssertExpectedStartingStateOfFileSystem();
}

TEST(RecursivelyDeleteAllFilesInDirectory_DryRunIsFalse_RecursivelyPrintsWouldDeleteFile)
{
  AssertExpectedStartingStateOfFileSystem();
  FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
  args.dryrun = false;
  //
  _recursiveFileDeleter.RecursivelyDeleteAllFilesInDirectory(_rootFolderPathString.c_str(), args);
  //
  AssertExpectedEndingStateOfFileSystem();
}

#if defined __linux__

// chmod 777 = "drwxrwxrwx"
static void Chmod777Directory(const fs::path& directoryPath)
{
  const int chmodReturnValue = chmod(directoryPath.string().c_str(), 0777);
  release_assert(chmodReturnValue == 0);
}

#endif

RUN_TESTS(RecursiveFileDeleterIntegrationTests)
