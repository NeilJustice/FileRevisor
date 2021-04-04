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
STRING_ENUM_MAP(FileExceptionType, Unset)
STRING_ENUM_MAP(FileExceptionType, FailedToOpenFile)
STRING_ENUM_MAP(FileExceptionType, FailedToCloseFile)
STRING_ENUM_MAP(FileExceptionType, FileIsMalformed)
STRING_ENUM_MAP(FileExceptionType, FailedToDeleteFile)
STRING_ENUM_MAP(FileExceptionType, FailedToRenameDirectory)
STRING_ENUM_MAP(FileExceptionType, FailedToDeleteDirectory)
STRING_ENUM_MAP(FileExceptionType, WindowsFailedToSetFileAttribute)
STRING_ENUM_MAP(FileExceptionType, WindowsFindFirstFileExReturnedInvalidHandleValue)
STRING_ENUM_MAP(FileExceptionType, MaxValue)
END_STRING_ENUM_MAP
