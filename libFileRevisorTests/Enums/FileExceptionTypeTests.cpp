#include "pch.h"
#include "libFileRevisor/Enums/FileExceptionType.h"

TESTS(FileExceptionTypeTests)
AFACT(EnumStringArray_ENUM_AS_STRING_ReturnsExpectedStringForEachEnumValue)
EVIDENCE

TEST(EnumStringArray_ENUM_AS_STRING_ReturnsExpectedStringForEachEnumValue)
{
   ARE_EQUAL("Unset", ENUM_AS_STRING(FileExceptionType, FileExceptionType::Unset));
   ARE_EQUAL("FailedToOpenFile", ENUM_AS_STRING(FileExceptionType, FileExceptionType::FailedToOpenFile));
   ARE_EQUAL("FailedToCloseFile", ENUM_AS_STRING(FileExceptionType, FileExceptionType::FailedToCloseFile));
   ARE_EQUAL("FileIsMalformed", ENUM_AS_STRING(FileExceptionType, FileExceptionType::FileIsMalformed));
   ARE_EQUAL("FailedToDeleteFile", ENUM_AS_STRING(FileExceptionType, FileExceptionType::FailedToDeleteFile));
   ARE_EQUAL("FailedToRenameDirectory", ENUM_AS_STRING(FileExceptionType, FileExceptionType::FailedToRenameDirectory));
   ARE_EQUAL("FailedToDeleteDirectory", ENUM_AS_STRING(FileExceptionType, FileExceptionType::FailedToDeleteDirectory));
   ARE_EQUAL("WindowsFailedToSetFileAttribute", ENUM_AS_STRING(FileExceptionType, FileExceptionType::WindowsFailedToSetFileAttribute));
   ARE_EQUAL("WindowsFindFirstFileExReturnedInvalidHandleValue", ENUM_AS_STRING(FileExceptionType, FileExceptionType::WindowsFindFirstFileExReturnedInvalidHandleValue));
   ARE_EQUAL("MaxValue", ENUM_AS_STRING(FileExceptionType, FileExceptionType::MaxValue));
}

RUN_TESTS(FileExceptionTypeTests)
