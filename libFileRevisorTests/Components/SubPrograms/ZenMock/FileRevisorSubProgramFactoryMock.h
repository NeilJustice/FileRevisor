#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgramFactory.h"

class FileRevisorSubProgramFactoryMock : public Zen::Mock<FileRevisorSubProgramFactory>
{
public:
   ZENMOCK_NONVOID1_CONST(shared_ptr<FileRevisorSubProgram>, NewFileRevisorSubProgram, ProgramMode)
};
