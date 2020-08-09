#include "pch.h"
#include "libFileRevisor/Enums/ProgramMode.h"

TESTS(ProgramModeTests)
AFACT(OStreamLeftShiftOperator_ProgramModeIsOutsideOfValidEnumValues_WritesProgramModeAsInt)
AFACT(OStreamLeftShiftOperator_ProgramModeIsWithinValidEnumValues_WritesProgramModeAsString)
EVIDENCE

TEST(OStreamLeftShiftOperator_ProgramModeIsOutsideOfValidEnumValues_WritesProgramModeAsInt)
{
   ostringstream oss;
   const ProgramMode firstProgramModeValue =
      static_cast<ProgramMode>(ZenUnit::RandomBetween<int>(10, 20));
   const ProgramMode secondProgramModeValue =
      static_cast<ProgramMode>(ZenUnit::RandomBetween<int>(10, 20));
   //
   oss << firstProgramModeValue << secondProgramModeValue;
   //
   const string ossString = oss.str();
   const string expectedOssString =
      to_string(static_cast<int>(firstProgramModeValue)) +
      to_string(static_cast<int>(secondProgramModeValue));
   ARE_EQUAL(expectedOssString, ossString);
}

TEST(OStreamLeftShiftOperator_ProgramModeIsWithinValidEnumValues_WritesProgramModeAsString)
{
   ostringstream oss;
   const ProgramMode firstProgramModeValue =
      ZenUnit::RandomEnum<ProgramMode>(ProgramMode::MaxValue);
   const ProgramMode secondProgramModeValue =
      ZenUnit::RandomEnum<ProgramMode>(ProgramMode::MaxValue);
   //
   oss << firstProgramModeValue << secondProgramModeValue;
   //
   const string ossString = oss.str();
   const string expectedOssString =
      ENUM_TO_STRING(ProgramMode, firstProgramModeValue) +
      ENUM_TO_STRING(ProgramMode, secondProgramModeValue);
   ARE_EQUAL(expectedOssString, ossString);
}

RUN_TESTS(ProgramModeTests)
