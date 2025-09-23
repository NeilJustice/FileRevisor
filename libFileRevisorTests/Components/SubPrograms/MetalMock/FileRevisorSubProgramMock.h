#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

class FileRevisorSubProgramMock : public Metal::Mock<FileRevisorSubProgram>
{
public:
   METALMOCK_VOID1(Initialize, const FileRevisorArgs&)
   // Actions
   METALMOCK_NONVOID0_CONST(int, Run)
};
