#pragma once
#include "libFileRevisor/Components/FileSystem/RecursiveFileDeleter.h"

class RecursiveFileDeleterMock : public Zen::Mock<RecursiveFileDeleter>
{
public:
   ZENMOCK_VOID2_CONST(RecursivelyDeleteAllFilesInDirectory, const char*, const FileRevisorArgs&)
#ifdef _WIN32
   ZENMOCK_VOID1_CONST(RemoveReadonlyFlagFromFileSystemFilePath, const fs::path&)
   ZENMOCK_VOID1_CONST(RemoveReadonlyFlagFromConstCharPointerFilePath, const char*)
#endif
};
