#pragma once
#include "libFileRevisor/Components/FileSystem/FileSystem.h"

class FileSystemMock : public Metal::Mock<FileSystem>
{
public:
   // Deletes
   METALMOCK_VOID4_CONST(DeleteTopLevelFilesAndEmptyDirectoriesInDirectory, const fs::path&, bool, bool, bool)
   METALMOCK_VOID2_CONST(RecursivelyDeleteAllFilesInDirectory, const string&, const FileRevisorArgs&)
   METALMOCK_VOID2_CONST(RemoveFile, const char*, bool)
   METALMOCK_VOID4_CONST(DeleteFileOrDirectory, const fs::path&, bool, bool, bool)

   // Open File
   METALMOCK_NONVOID2_CONST(shared_ptr<FILE>, OpenFile, const fs::path&, const char*)

   // Queries
   METALMOCK_NONVOID1_CONST(fs::path, GetAbsolutePath, const fs::path&)
   METALMOCK_NONVOID0_CONST(fs::path, CurrentFolderPath)
   METALMOCK_NONVOID2_CONST(vector<fs::path>, GetFilePathsInDirectory, const fs::path&, bool)
   METALMOCK_NONVOID2_CONST(vector<fs::path>, GetFolderPathsInDirectory, const fs::path&, bool)
   METALMOCK_NONVOID2_CONST(vector<string>, GetStringFolderPathsInDirectory, const fs::path&, bool)
   METALMOCK_NONVOID1_CONST(bool, FileOrDirectoryExists, const fs::path&)

   // Reads
   METALMOCK_NONVOID1_CONST(std::string, ReadText, const fs::path&)

   // Readonly Flags
   METALMOCK_VOID2_CONST(RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows, const fs::path&, bool)

   // Writes
   METALMOCK_VOID2_CONST(CreateTextFile, const fs::path&, string_view)
   METALMOCK_NONVOID2_CONST(fs::path, RenameFile, const fs::path&, string_view)
   METALMOCK_NONVOID2_CONST(fs::path, RenameDirectory, const fs::path&, string_view)
};
