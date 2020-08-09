#pragma once

struct RenameResult
{
public:
   bool didRenameFileOrDirectory;
   fs::path originalFileOrDirectoryPath;
   fs::path renamedFileOrDirectoryPath;

   RenameResult();
   RenameResult(bool didRenameFileOrDirectory, const fs::path& originalFileOrDirectoryPath, const fs::path& renamedFileOrDirectoryPath);
   static bool DidRenameFileOrDirectoryFieldIsTrue(const RenameResult& renameResult);
};
