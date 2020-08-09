#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

class FileRevisorSubProgramMock : public Zen::Mock<FileRevisorSubProgram>
{
public:
   ZENMOCK_NONVOID1_CONST(int, Run, const FileRevisorArgs&)
};
