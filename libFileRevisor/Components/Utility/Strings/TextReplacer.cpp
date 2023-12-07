#include "pch.h"
#include "libFileRevisor/Components/Utility/Strings/TextReplacer.h"

string ReplaceAllSubstrings(string_view str, string_view substring, string_view replacement)
{
   if (substring.empty())
   {
      throw invalid_argument("Utils::ReplaceAllSubstrings(string_view str, string_view substring, string_view replacement) called with empty substring");
   }
   string replacedString(str);
   size_t runningFindPosition = 0;
   while (true)
   {
      const char* const strstrResult = strstr(replacedString.data() + runningFindPosition, substring.data());
      if (strstrResult == nullptr)
      {
         return replacedString;
      }
      const size_t findPosition = static_cast<size_t>(strstrResult - replacedString.data());
      replacedString.replace(findPosition, substring.length(), replacement);
      runningFindPosition += findPosition + substring.size();
   }
   return replacedString;
}

string TextReplacer::ReplaceText(string_view str, string_view substring, string_view replacement) const
{
   string replacedString = ReplaceAllSubstrings(str, substring, replacement);
   return replacedString;
}
