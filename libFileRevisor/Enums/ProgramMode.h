#pragma once
#include "libFileRevisor/Utilities/STRING_ENUM_MAP.h"

enum class ProgramMode
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
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(ProgramMode, Unset)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(ProgramMode, RenameFiles)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(ProgramMode, RenameDirectories)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(ProgramMode, ReplaceTextInTextFiles)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(ProgramMode, DeleteDirectory)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(ProgramMode, MaxValue)
END_STRING_ENUM_MAP
