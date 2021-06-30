#pragma once
#include "libFileRevisor/StaticUtilities/STRING_ENUM_MAP.h"

enum class ProgramMode : unsigned char
{
   Unset,
   RenameFiles,
   RenameDirectories,
   ReplaceTextInTextFiles,
   DeleteDirectory,
   MaxValue
};

ostream& operator<<(ostream& os, ProgramMode programMode);

BEGIN_STRING_ENUM_MAP(ProgramMode)
STRING_ENUM_MAP(ProgramMode, Unset)
STRING_ENUM_MAP(ProgramMode, RenameFiles)
STRING_ENUM_MAP(ProgramMode, RenameDirectories)
STRING_ENUM_MAP(ProgramMode, ReplaceTextInTextFiles)
STRING_ENUM_MAP(ProgramMode, DeleteDirectory)
STRING_ENUM_MAP(ProgramMode, MaxValue)
END_STRING_ENUM_MAP
