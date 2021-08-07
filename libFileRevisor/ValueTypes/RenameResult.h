#pragma once

struct RenameResult
{
public:
   bool didRenameFileOrDirectory;
   fs::path originalFileOrDirectoryPath;
   fs::path renamedFileOrDirectoryPath;

   RenameResult();
   RenameResult(
      bool didRenameFileOrDirectory,
      const fs::path& originalFileOrDirectoryPath,
      const fs::path& renamedFileOrDirectoryPath);
   static bool DidRenameFileOrDirectoryFieldIsTrue(const RenameResult& renameResult);
};

#ifdef _WIN32
#ifdef _DEBUG
static_assert(sizeof(RenameResult) == 88);
#else
static_assert(sizeof(RenameResult) == 72);
#endif
#else
static_assert(sizeof(RenameResult) == 88);
#endif
