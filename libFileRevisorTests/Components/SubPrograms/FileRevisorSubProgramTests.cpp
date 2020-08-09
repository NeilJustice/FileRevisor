#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

TESTS(FileRevisorSubProgramTests)
AFACT(DefaultConstructor_NewsComponents)
AFACT(Run_Returns0)
EVIDENCE

TEST(DefaultConstructor_NewsComponents)
{
   FileRevisorSubProgram fileRevisorSubProgram;
   DELETE_TO_ASSERT_NEWED(fileRevisorSubProgram._protected_console);
   DELETE_TO_ASSERT_NEWED(fileRevisorSubProgram._protected_fileSystem);
   DELETE_TO_ASSERT_NEWED(fileRevisorSubProgram._protected_pluralizer);
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
