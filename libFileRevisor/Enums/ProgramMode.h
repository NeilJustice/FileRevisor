#pragma once
#include "libFileRevisor/StaticUtilities/ENUM_STRING_ARRAY.h"

enum struct ProgramMode
{
   Unset,
   RenameFiles,
   RenameDirectories,
   ReplaceTextInTextFiles,
   DeleteDirectory,
   MaxValue
};

BEGIN_ENUM_STRING_ARRAY(ProgramMode)
ENUM_STRING_ARRAY_ELEMENT(Unset)
ENUM_STRING_ARRAY_ELEMENT(RenameFiles)
ENUM_STRING_ARRAY_ELEMENT(RenameDirectories)
ENUM_STRING_ARRAY_ELEMENT(ReplaceTextInTextFiles)
ENUM_STRING_ARRAY_ELEMENT(DeleteDirectory)
ENUM_STRING_ARRAY_ELEMENT(MaxValue)
END_ENUM_STRING_ARRAY
