#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

TESTS(FileRevisorSubProgramTests)
AFACT(Run_Returns0)
EVIDENCE

TEST(Run_Returns0)
{
   FileRevisorSubProgram fileRevisorSubProgram;
   const FileRevisorArgs fileRevisorArgs;
   //
   const int runReturnValue = fileRevisorSubProgram.Run(fileRevisorArgs);
   //
   IS_ZERO(runReturnValue);
}

RUN_TESTS(FileRevisorSubProgramTests)
