#include "pch.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"

RenameResult::RenameResult()
   : didRenameFileOrDirectory(false)
{
}

RenameResult::RenameResult(
   bool didRenameFileOrDirectory,
   const fs::path& originalFileOrFolderPath,
   const fs::path& renamedFileOrFolderPath)
   : didRenameFileOrDirectory(didRenameFileOrDirectory)
   , originalFileOrFolderPath(originalFileOrFolderPath)
   , renamedFileOrFolderPath(renamedFileOrFolderPath)
{
}

bool RenameResult::DidRenameFileOrDirectoryFieldIsTrue(const RenameResult& renameResult)
{
   return renameResult.didRenameFileOrDirectory;
}
