#pragma once
#include "libFileRevisor/Components/FileRevisor/FileRevisorArgsParser.h"

class FileRevisorArgsParserMock : public Zen::Mock<FileRevisorArgsParser>
{
public:
   ZENMOCK_NONVOID1_CONST(FileRevisorArgs, ParseArgs, const vector<string>&)
   ZENMOCK_VOID1_CONST(PrintPreamble, const FileRevisorArgs&)
};
