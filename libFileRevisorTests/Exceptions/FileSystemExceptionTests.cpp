#include "pch.h"
#include "libFileRevisor/Exceptions/FileSystemException.h"
#include "libFileRevisor/Enums/FileExceptionType.h"

TESTS(FileExceptionTests)
AFACT(Constructor_MakesWhatReturnExpectedExceptionMessage)
EVIDENCE

TEST(Constructor_MakesWhatReturnExpectedExceptionMessage)
{
   const FileExceptionType fileExceptionType =
      ZenUnit::RandomEnum<FileExceptionType>(FileExceptionType::MaxValue);
   const string exceptionMessage = ZenUnit::Random<string>();
   //
   const FileSystemException fileException(fileExceptionType, exceptionMessage);
   const char* const fullExceptionMessage = fileException.what();
   //
   const string expectedFullExceptionMessage =
      ENUM_TO_STRING(FileExceptionType, fileExceptionType) + ": " + string(exceptionMessage);
   ARE_EQUAL(expectedFullExceptionMessage, fullExceptionMessage);
}

RUN_TESTS(FileExceptionTests)
