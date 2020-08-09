#pragma once
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"

class FileRevisorPreambleMakerMock : public Zen::Mock<FileRevisorPreambleMaker>
{
public:
   ZENMOCK_NONVOID1_CONST(string, MakePreambleLines, const FileRevisorArgs&)
};
