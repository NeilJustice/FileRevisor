#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

class FileRevisorSubProgramMock : public Zen::Mock<FileRevisorSubProgram>
{
public:
   METALMOCK_NONVOID1_CONST(int, Run, const FileRevisorArgs&)
};
