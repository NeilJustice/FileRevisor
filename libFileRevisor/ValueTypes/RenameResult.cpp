#include "pch.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"

RenameResult::RenameResult()
   : didRenameFileOrDirectory(false)
{
}

RenameResult::RenameResult(
   bool didRenameFileOrDirectory,
   const fs::path& originalFileOrDirectoryPath,
   const fs::path& renamedFileOrDirectoryPath)
   : didRenameFileOrDirectory(didRenameFileOrDirectory)
   , originalFileOrDirectoryPath(originalFileOrDirectoryPath)
   , renamedFileOrDirectoryPath(renamedFileOrDirectoryPath)
{
}

bool RenameResult::DidRenameFileOrDirectoryFieldIsTrue(const RenameResult& renameResult)
{
   return renameResult.didRenameFileOrDirectory;
}
