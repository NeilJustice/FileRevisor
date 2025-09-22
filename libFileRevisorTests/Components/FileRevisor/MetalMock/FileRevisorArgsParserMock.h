#pragma once
#include "libFileRevisor/Components/FileRevisor/FileRevisorArgsParser.h"

class FileRevisorArgsParserMock : public Metal::Mock<FileRevisorArgsParser>
{
public:
   METALMOCK_NONVOID1_CONST(FileRevisorArgs, ParseStringArgs, const vector<string>&)
   METALMOCK_VOID1_CONST(PrintPreambleLines, const FileRevisorArgs&)
};
