#include "pch.h"
#include "libFileRevisor/UtilityComponents/DataStructures/CharArray64Helper.h"

bool CharArray64Helper::ArrayContains(const array<char, 64>& /*chars*/, char /*c*/, size_t /*maximumNumberOfElementsToCompare*/) const
{
   // for (size_t i = 0; i < numberOfBytesRead; ++i)
   // {
   //    const char ithFileByte = first1KBytesInFile[i];
   //    if (ithFileByte == 0)
   //    {
   //       return true;
   //    }
   // }
   return false;
}
