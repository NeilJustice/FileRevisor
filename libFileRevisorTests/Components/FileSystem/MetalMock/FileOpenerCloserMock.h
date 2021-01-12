#pragma once
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"

class FileOpenerCloserMock : public Metal::Mock<FileOpenerCloser>
{
public:
   METALMOCK_VOID2_CONST(CloseFile, FILE*, const fs::path&)
   METALMOCK_NONVOID1_CONST(FILE*, CreateWriteModeBinaryFile, const fs::path&)
   METALMOCK_NONVOID1_CONST(FILE*, CreateWriteModeTextFile, const fs::path&)
   METALMOCK_NONVOID2_CONST(FILE*, OpenReadModeBinaryFile, const fs::path&, bool)
   METALMOCK_NONVOID1_CONST(FILE*, OpenReadModeTextFile, const fs::path&)
   METALMOCK_NONVOID1_CONST(FILE*, OpenAppendModeTextFile, const fs::path&)
};
