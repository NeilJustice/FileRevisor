#pragma once
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"

class FileRevisorPreambleMakerMock : public Metal::Mock<FileRevisorPreambleMaker>
{
public:
   METALMOCK_VOID1_CONST(PrintPreambleLines, const FileRevisorArgs&)
};
