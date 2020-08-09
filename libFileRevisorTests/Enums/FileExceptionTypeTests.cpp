#include "pch.h"
#include "libFileRevisor/Enums/FileExceptionType.h"

TESTS(FileExceptionTypeTests)
AFACT(OStreamLeftShiftOperator_FileExceptionTypeValueIsNotWithinRange_WritesFileExceptionTypeAsInt)
AFACT(OStreamLeftShiftOperator_FileExceptionTypeValueIsWithinRange_WritesFileExceptionTypeAsString)
EVIDENCE

TEST(OStreamLeftShiftOperator_FileExceptionTypeValueIsNotWithinRange_WritesFileExceptionTypeAsInt)
{
   ostringstream oss;
   const FileExceptionType firstFileExceptionTypeValue =
      static_cast<FileExceptionType>(ZenUnit::RandomBetween<int>(10, 20));
   const FileExceptionType secondFileExceptionTypeValue =
      static_cast<FileExceptionType>(ZenUnit::RandomBetween<int>(10, 20));
   //
   oss << firstFileExceptionTypeValue << secondFileExceptionTypeValue;
   //
   const string ossString = oss.str();
   const string expectedOssString =
      to_string(static_cast<int>(firstFileExceptionTypeValue)) +
      to_string(static_cast<int>(secondFileExceptionTypeValue));
   ARE_EQUAL(expectedOssString, ossString);
}

TEST(OStreamLeftShiftOperator_FileExceptionTypeValueIsWithinRange_WritesFileExceptionTypeAsString)
{
   ostringstream oss;
   const FileExceptionType firstFileExceptionTypeValue =
      ZenUnit::RandomEnum<FileExceptionType>(FileExceptionType::MaxValue);
   const FileExceptionType secondFileExceptionTypeValue =
      ZenUnit::RandomEnum<FileExceptionType>(FileExceptionType::MaxValue);
   //
   oss << firstFileExceptionTypeValue << secondFileExceptionTypeValue;
   //
   const string ossString = oss.str();
   const string expectedOssString =
      ENUM_TO_STRING(FileExceptionType, firstFileExceptionTypeValue) +
      ENUM_TO_STRING(FileExceptionType, secondFileExceptionTypeValue);
   ARE_EQUAL(expectedOssString, ossString);
}

RUN_TESTS(FileExceptionTypeTests)
