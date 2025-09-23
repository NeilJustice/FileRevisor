#pragma once
#include "libFileRevisor/StaticUtilities/ENUM_STRING_ARRAY.h"

enum struct FileExceptionType : unsigned char
{
   Unset,
   FailedToOpenFile,
   FailedToCloseFile,
   FileIsMalformed,
   FailedToDeleteFile,
   FailedToRenameDirectory,
   FailedToDeleteDirectory,
   WindowsFailedToSetFileAttribute,
   WindowsFindFirstFileExReturnedInvalidHandleValue,
   MaxValue
};

BEGIN_ENUM_STRING_ARRAY(FileExceptionType)
ENUM_STRING_ARRAY_ELEMENT(Unset)
ENUM_STRING_ARRAY_ELEMENT(FailedToOpenFile)
ENUM_STRING_ARRAY_ELEMENT(FailedToCloseFile)
ENUM_STRING_ARRAY_ELEMENT(FileIsMalformed)
ENUM_STRING_ARRAY_ELEMENT(FailedToDeleteFile)
ENUM_STRING_ARRAY_ELEMENT(FailedToRenameDirectory)
ENUM_STRING_ARRAY_ELEMENT(FailedToDeleteDirectory)
ENUM_STRING_ARRAY_ELEMENT(WindowsFailedToSetFileAttribute)
ENUM_STRING_ARRAY_ELEMENT(WindowsFindFirstFileExReturnedInvalidHandleValue)
ENUM_STRING_ARRAY_ELEMENT(MaxValue)
END_ENUM_STRING_ARRAY
