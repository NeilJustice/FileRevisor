#include "pch.h"
#include "libFileRevisor/Enums/FileExceptionType.h"
#include "libFileRevisor/Exceptions/FileSystemException.h"

TESTS(FileExceptionTests)
AFACT(TwoArgConstructor_MakesWhatReturnExpectedExceptionMessage)
EVIDENCE

TEST(TwoArgConstructor_MakesWhatReturnExpectedExceptionMessage)
{
   const FileExceptionType fileExceptionType = ZenUnit::RandomEnum<FileExceptionType>();
   const string exceptionMessage = ZenUnit::Random<string>();
   //
   const FileSystemException fileException(fileExceptionType, exceptionMessage);
   const char* const fullExceptionMessage = fileException.what();
   //
   const string expectedFullExceptionMessage = ENUM_AS_STRING(FileExceptionType, fileExceptionType) + ": "s + string(exceptionMessage);
   ARE_EQUAL(expectedFullExceptionMessage, fullExceptionMessage);
}

RUN_TESTS(FileExceptionTests)
