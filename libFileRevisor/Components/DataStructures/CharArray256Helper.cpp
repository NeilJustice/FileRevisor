#include "pch.h"
#include "libFileRevisor/Components/DataStructures/CharArray256Helper.h"

bool CharArray256Helper::ArrayContains0(const array<char, 256>& chars, size_t maximumNumberOfElementsToCompare) const
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
