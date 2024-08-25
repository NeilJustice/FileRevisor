#pragma once
#include "libFileRevisor/Components/FileSystem/RecursiveFileDeleter.h"

class RecursiveFileDeleterMock : public Metal::Mock<RecursiveFileDeleter>
{
public:
   METALMOCK_VOID2_CONST(RecursivelyDeleteAllFilesInDirectory, const char*, const FileRevisorArgs&)
#ifdef _WIN32
   METALMOCK_VOID1_CONST(RemoveReadonlyFlagFromFileSystemFilePath, const fs::path&)
   METALMOCK_VOID1_CONST(RemoveReadonlyFlagFromConstCharPointerFilePath, const char*)
#endif
};
