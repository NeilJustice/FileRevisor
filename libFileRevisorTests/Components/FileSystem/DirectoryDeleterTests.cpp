#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryDeleter.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"

TESTS(DirectoryDeleterTests)
AFACT(DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory_ContentsOnlyTrue_DoesNothing)
AFACT(DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory_ContentsOnlyFalse_TargetDirectoryPathIsNotCurrentDirectoryPath_DoesNothing)
AFACT(DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory_ContentsOnlyFalse_TargetDirectoryPathIsNotCurrentDirectoryPath_DeletesTargetDirectory)
EVIDENCE

DirectoryDeleter _directoryDeleter;
// Constant Components
FileSystemMock* _fileSystemMock = nullptr;

STARTUP
{
   // Constant Components
   _directoryDeleter._fileSystem.reset(_fileSystemMock = new FileSystemMock);
}

TEST(DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory_ContentsOnlyTrue_DoesNothing)
{
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.contentsOnly = true;
   //
   _directoryDeleter.DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory(args);
}

TEST(DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory_ContentsOnlyFalse_TargetDirectoryPathIsNotCurrentDirectoryPath_DoesNothing)
{
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.contentsOnly = false;
   _fileSystemMock->CurrentDirectoryPathMock.Return(args.targetDirectoryPath);
   //
   _directoryDeleter.DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory(args);
   //
   METALMOCK(_fileSystemMock->CurrentDirectoryPathMock.CalledOnce());
}

TEST(DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory_ContentsOnlyFalse_TargetDirectoryPathIsNotCurrentDirectoryPath_DeletesTargetDirectory)
{
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.contentsOnly = false;
   const fs::path currentDirectoryPath = ZenUnit::RandomNotEqualTo<fs::path>(args.targetDirectoryPath);
   _fileSystemMock->CurrentDirectoryPathMock.Return(currentDirectoryPath);
   _fileSystemMock->DeleteFileOrDirectoryMock.Expect();
   //
   _directoryDeleter.DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory(args);
   //
   METALMOCKTHEN(_fileSystemMock->CurrentDirectoryPathMock.CalledOnce()).Then(
   METALMOCKTHEN(_fileSystemMock->DeleteFileOrDirectoryMock.CalledOnceWith(
      args.targetDirectoryPath,
      args.skipFilesInUse,
      args.dryrun,
      args.quiet)));
}

RUN_TESTS(DirectoryDeleterTests)
