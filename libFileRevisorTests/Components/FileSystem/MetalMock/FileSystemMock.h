#pragma once
#include "libFileRevisor/Components/FileSystem/FileSystem.h"

class FileSystemMock : public Metal::Mock<FileSystem>
{
public:
   // Queries
   METALMOCK_NONVOID1_CONST(fs::path, GetAbsolutePath, const fs::path&)
   METALMOCK_NONVOID0_CONST(fs::path, CurrentDirectoryPath)
   METALMOCK_NONVOID2_CONST(vector<fs::path>, GetFilePathsInDirectory, const fs::path&, bool)
   METALMOCK_NONVOID2_CONST(vector<fs::path>, GetDirectoryPathsInDirectory, const fs::path&, bool)
   METALMOCK_NONVOID2_CONST(vector<string>, GetStringDirectoryPathsInDirectory, const fs::path&, bool)
   METALMOCK_NONVOID1_CONST(bool, FileOrDirectoryExists, const fs::path&)

   // Reads
   METALMOCK_NONVOID1_CONST(std::string, ReadText, const fs::path&)

   // Writes
   METALMOCK_VOID2_CONST(CreateTextFile, const fs::path&, string_view)
   METALMOCK_NONVOID2_CONST(fs::path, RenameFile, const fs::path&, string_view)
   METALMOCK_NONVOID2_CONST(fs::path, RenameDirectory, const fs::path&, string_view)

   // Deletes
   METALMOCK_VOID2_CONST(DeleteDirectoryIfNotDryRun, const fs::path&, bool)
   METALMOCK_VOID1_CONST(RemoveFile, const char*)
   METALMOCK_VOID1_CONST(RemoveAll, const fs::path&)
   METALMOCK_VOID2_CONST(RecursivelyDeleteAllFilesInDirectory, const string&, const FileRevisorArgs&)

   // Readonly Flags
   METALMOCK_VOID1_CONST(RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows, const fs::path&)

   // Open And Close Files
   METALMOCK_NONVOID2_CONST(shared_ptr<FILE>, OpenFile, const fs::path&, const char*)
};
