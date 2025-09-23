#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

TESTS(FileRevisorSubProgramTests)
AFACT(DefaultConstructor_NewsComponents)
AFACT(Run_Returns0)
EVIDENCE

TEST(DefaultConstructor_NewsComponents)
{
   FileRevisorSubProgram fileRevisorSubProgram;
   // Base Class Constant Components
   DELETE_TO_ASSERT_NEWED(fileRevisorSubProgram.p_console);
   DELETE_TO_ASSERT_NEWED(fileRevisorSubProgram.p_fileSystem);
   DELETE_TO_ASSERT_NEWED(fileRevisorSubProgram.p_pluralizer);
}

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
