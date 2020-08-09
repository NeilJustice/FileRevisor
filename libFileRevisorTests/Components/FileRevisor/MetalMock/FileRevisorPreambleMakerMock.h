#pragma once
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"

class FileRevisorPreambleMakerMock : public Zen::Mock<FileRevisorPreambleMaker>
{
public:
   METALMOCK_NONVOID1_CONST(string, MakePreambleLines, const FileRevisorArgs&)
};
