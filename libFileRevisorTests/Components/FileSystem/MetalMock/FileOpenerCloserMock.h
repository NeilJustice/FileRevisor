#pragma once
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"

class FileOpenerCloserMock : public Metal::Mock<FileOpenerCloser>
{
public:
   METALMOCK_VOID2_CONST(CloseFile, FILE*, const fs::path&)
   METALMOCK_NONVOID2_CONST(shared_ptr<FILE>, OpenReadModeBinaryFile, const fs::path&, bool)
};
