#include "pch.h"
#include "libFileRevisor/Enums/ProgramMode.h"

TESTS(ProgramModeTests)
AFACT(EnumStringArray_ENUM_AS_STRING_ReturnsExpectedStringForEachEnumValue)
EVIDENCE

TEST(EnumStringArray_ENUM_AS_STRING_ReturnsExpectedStringForEachEnumValue)
{
   ARE_EQUAL("Unset", ENUM_AS_STRING(ProgramMode, ProgramMode::Unset));
   ARE_EQUAL("RenameFiles", ENUM_AS_STRING(ProgramMode, ProgramMode::RenameFiles));
   ARE_EQUAL("RenameDirectories", ENUM_AS_STRING(ProgramMode, ProgramMode::RenameDirectories));
   ARE_EQUAL("ReplaceTextInTextFiles", ENUM_AS_STRING(ProgramMode, ProgramMode::ReplaceTextInTextFiles));
   ARE_EQUAL("DeleteDirectory", ENUM_AS_STRING(ProgramMode, ProgramMode::DeleteDirectory));
   ARE_EQUAL("MaxValue", ENUM_AS_STRING(ProgramMode, ProgramMode::MaxValue));
}

RUN_TESTS(ProgramModeTests)
