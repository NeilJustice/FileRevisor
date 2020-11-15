#pragma once

enum class FileExceptionType : unsigned char
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

ostream& operator<<(ostream& os, FileExceptionType fileExceptionType);

BEGIN_STRING_ENUM_MAP(FileExceptionType)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(FileExceptionType, Unset)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(FileExceptionType, FailedToOpenFile)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(FileExceptionType, FailedToCloseFile)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(FileExceptionType, FileIsMalformed)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(FileExceptionType, FailedToDeleteFile)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(FileExceptionType, FailedToRenameDirectory)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(FileExceptionType, FailedToDeleteDirectory)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(FileExceptionType, WindowsFailedToSetFileAttribute)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(FileExceptionType, WindowsFindFirstFileExReturnedInvalidHandleValue)
ADD_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPPINGS(FileExceptionType, MaxValue)
END_STRING_ENUM_MAP
