#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

TESTS(FileRevisorSubProgramTests)
AFACT(DefaultConstructor_SetsFieldsToDefaultValues)
AFACT(Initialize_DoesSo)
// Actions
AFACT(Run_Returns0)
EVIDENCE

FileRevisorSubProgram _fileRevisorSubProgram;

TEST(DefaultConstructor_SetsFieldsToDefaultValues)
{
   const FileRevisorSubProgram fileRevisorSubProgram;
   IS_DEFAULT_VALUE(fileRevisorSubProgram.p_args);
}

TEST(Initialize_DoesSo)
{
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   _fileRevisorSubProgram.Initialize(args);
   //
   ARE_EQUAL(args, _fileRevisorSubProgram.p_args);
}

// Actions

TEST(Run_Returns0)
{
   FileRevisorSubProgram fileRevisorSubProgram;
   //
   const int runReturnValue = fileRevisorSubProgram.Run();
   //
   IS_ZERO(runReturnValue);
}

RUN_TESTS(FileRevisorSubProgramTests)
