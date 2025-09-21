#include "pch.h"
#include "libFileRevisor/Components/Strings/Pluralizer.h"

string Pluralizer::PotentiallyPluralizeWord(size_t count, string_view singularWord, string_view pluralWord) const
{
   if (count == 1)
   {
      return string(singularWord);
   }
   return string(pluralWord);
}
