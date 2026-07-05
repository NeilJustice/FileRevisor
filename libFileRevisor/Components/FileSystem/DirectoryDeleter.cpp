#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/FileSystem/DirectoryDeleter.h"

DirectoryDeleter::DirectoryDeleter()
   // Constant Components
   : _fileSystem(make_unique<FileSystem>())
{
}

DirectoryDeleter::~DirectoryDeleter()
{
}

void DirectoryDeleter::DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory(const FileRevisorArgs& args) const
{
   if (args.contentsOnly)
   {
      return;
   }
   const fs::path currentDirectoryPath = _fileSystem->CurrentDirectoryPath();
   if (args.targetDirectoryPath != currentDirectoryPath)
   {
      _fileSystem->DeleteFileOrDirectory(
         args.targetDirectoryPath,
         args.skipFilesInUse,
         args.dryrun,
         args.quiet);
   }
}