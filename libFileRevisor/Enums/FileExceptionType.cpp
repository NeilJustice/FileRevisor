#include "pch.h"
#include "libFileRevisor/Enums/FileExceptionType.h"

ostream& operator<<(ostream& os, FileExceptionType fileExceptionType)
{
   if (fileExceptionType < FileExceptionType::Unset || fileExceptionType > FileExceptionType::MaxValue)
   {
      os << static_cast<int>(fileExceptionType);
   }
   else
   {
      const string fileExceptionTypeAsString = ENUM_TO_STRING(FileExceptionType, fileExceptionType);
      os << fileExceptionTypeAsString;
   }
   return os;
}

INITIALIZE_ENUM_TO_STRING_AND_STRING_TO_ENUM_MAPS(FileExceptionType)
