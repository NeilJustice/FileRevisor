#include "pch.h"
#include "libFileRevisor/Enums/ProgramMode.h"

ostream& operator<<(ostream& os, ProgramMode programMode)
{
   if (programMode < ProgramMode::Unset || programMode > ProgramMode::MaxValue)
   {
      os << static_cast<int>(programMode);
   }
   else
   {
      const string programModeAsString = ENUM_TO_STRING(ProgramMode, programMode);
      os << programModeAsString;
   }
   return os;
}

INITIALIZE_STRING_ENUM_MAP(ProgramMode)
