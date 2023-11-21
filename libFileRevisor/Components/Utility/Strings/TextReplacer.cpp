#include "pch.h"
#include "libFileRevisor/Components/Utility/Strings/TextReplacer.h"

string ReplaceAllSubstrings(string_view str, string_view substring, string_view replacement)
{
   if (substring.empty())
   {
      throw invalid_argument("Utils::ReplaceAllSubstrings(string_view str, string_view substring, string_view replacement) called with empty substring");
   }
   string replacedString(str);
   while (true)
   {
      const size_t findPosition = replacedString.find(substring);
      if (findPosition == string::npos)
      {
         return replacedString;
      }
      replacedString.replace(findPosition, substring.length(), replacement);
   }
   return replacedString;
}

string TextReplacer::ReplaceText(string_view str, string_view substring, string_view replacement) const
{
   string replacedString = ReplaceAllSubstrings(str, substring, replacement);
   return replacedString;
}
