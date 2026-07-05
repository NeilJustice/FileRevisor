#pragma once
#include "libFileRevisor/Components/FileSystem/DirectoryDeleter.h"

class DirectoryDeleterMock : public Metal::Mock<DirectoryDeleter>
{
public:
   METALMOCK_VOID1_CONST(DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory, const FileRevisorArgs&)
};
