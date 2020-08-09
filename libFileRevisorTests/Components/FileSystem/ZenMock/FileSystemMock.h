#pragma once
#include "libFileRevisor/Components/FileSystem/FileSystem.h"

class FileSystemMock : public Zen::Mock<FileSystem>
{
public:
   // Queries
   ZENMOCK_NONVOID1_CONST(fs::path, GetAbsolutePath, const fs::path&)
   ZENMOCK_NONVOID0_CONST(fs::path, CurrentDirectoryPath)
   ZENMOCK_NONVOID2_CONST(vector<fs::path>, GetFilePathsInDirectory, const fs::path&, bool)
   ZENMOCK_NONVOID2_CONST(vector<fs::path>, GetNonEmptyTextFilePathsInDirectory, const fs::path&, bool)
   ZENMOCK_NONVOID2_CONST(vector<fs::path>, GetDirectoryPathsInDirectory, const fs::path&, bool)
   ZENMOCK_NONVOID2_CONST(vector<string>, GetStringDirectoryPathsInDirectory, const fs::path&, bool)
   ZENMOCK_NONVOID1_CONST(bool, FileOrDirectoryExists, const fs::path&)

   // Reads
   ZENMOCK_NONVOID1_CONST(std::string, ReadText, const fs::path&)

   // Writes
   ZENMOCK_VOID2_CONST(CreateTextFile, const fs::path&, string_view)
   ZENMOCK_NONVOID2_CONST(fs::path, RenameFile, const fs::path&, string_view)
   ZENMOCK_NONVOID2_CONST(fs::path, RenameDirectory, const fs::path&, string_view)

   // Deletes
   ZENMOCK_VOID1_CONST(RemoveFile, const char*)
   ZENMOCK_NONVOID1_CONST(unsigned long long, RemoveAll, const fs::path&)
   ZENMOCK_VOID2_CONST(RecursivelyDeleteAllFilesInDirectory, const string&, const FileRevisorArgs&)

   // Readonly Flags
   ZENMOCK_VOID1_CONST(RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows, const fs::path&)

   // File Handles
   ZENMOCK_VOID2_CONST(CloseFile, const fs::path&, FILE*)
};
