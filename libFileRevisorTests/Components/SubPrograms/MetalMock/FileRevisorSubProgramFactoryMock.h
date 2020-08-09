#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgramFactory.h"

class FileRevisorSubProgramFactoryMock : public Metal::Mock<FileRevisorSubProgramFactory>
{
public:
   METALMOCK_NONVOID1_CONST(shared_ptr<FileRevisorSubProgram>, NewFileRevisorSubProgram, ProgramMode)
};
