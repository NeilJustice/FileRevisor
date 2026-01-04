#pragma once

struct RenameResult
{
   bool didRenameFileOrDirectory;
   fs::path originalFileOrFolderPath;
   fs::path renamedFileOrFolderPath;

   RenameResult();
   RenameResult(
      bool didRenameFileOrDirectory,
      const fs::path& originalFileOrFolderPath,
      const fs::path& renamedFileOrFolderPath);
   static bool DidRenameFileOrDirectoryFieldIsTrue(const RenameResult& renameResult);
};

#ifdef _WIN32
   #ifdef _DEBUG
      static_assert(sizeof(RenameResult) == 88);
   #else
      static_assert(sizeof(RenameResult) == 72);
   #endif
#elifdef __linux__
   #ifdef _LIBCPP_VERSION
      static_assert(sizeof(RenameResult) == 56);
   #else
      static_assert(sizeof(RenameResult) == 88);
   #endif
#endif
