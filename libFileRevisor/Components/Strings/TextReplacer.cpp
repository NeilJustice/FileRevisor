#include "pch.h"
#include "libFileRevisor/Components/Strings/TextReplacer.h"

string TextReplacer::ReplaceText(
   string_view str,
   string_view substring,
   const string& replacement) const
{
   const regex escapedSubstring = EscapeRegexPattern(substring);
   const regex escapedReplacement = EscapeRegexPattern(replacement);
   string replacedString = regex_replace(str.data(), escapedSubstring, replacement);
   return replacedString;
}

// Private Functions

regex TextReplacer::EscapeRegexPattern(string_view regexPattern)
{
   static const char* const metacharacters = R"(\.^$-+()[]{}|?*)";
   string escapedRegexPattern;
   escapedRegexPattern.reserve(regexPattern.size());
   for (char c : regexPattern)
   {
      const char* const strchrReturnValue = strchr(metacharacters, c);
      if (strchrReturnValue != nullptr)
      {
         escapedRegexPattern.push_back('\\');
      }
      escapedRegexPattern.push_back(c);
   }
   regex escapedRegex(escapedRegexPattern);
   return escapedRegex;
}
