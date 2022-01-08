#include "pch.h"
#include "libFileRevisor/UtilityComponents/DataStructures/CharArray128Helper.h"

bool CharArray128Helper::ArrayContains0(const array<char, 128>& chars, size_t maximumNumberOfElementsToCompare) const
{
   for (size_t i = 0; i < maximumNumberOfElementsToCompare; ++i)
   {
      const char ithChar = chars[i];
      if (ithChar == 0)
      {
         return true;
      }
   }
   return false;
}
