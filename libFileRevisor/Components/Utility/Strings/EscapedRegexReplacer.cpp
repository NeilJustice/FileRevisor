#include "pch.h"
#include "libFileRevisor/Components/Utility/Strings/EscapedRegexReplacer.h"

string EscapedRegexReplacer::ReplaceText(string_view str, string_view matchPattern, string_view replacementPattern) const
{
   const regex regexMatchingPattern(matchPattern.data());
   string regexReplacedString = regex_replace(str.data(), regexMatchingPattern, replacementPattern.data());
   return regexReplacedString;
}

// Private Functions

string EscapedRegexReplacer::EscapeRegex(string_view regexPattern)
{
   static const char* const metacharacters = R"(\.^$-+()[]{}|?*)";
   string escapedRegexPattern;
   escapedRegexPattern.reserve(regexPattern.size());
   for (char c : regexPattern)
   {
      if (strchr(metacharacters, c))
      {
         escapedRegexPattern.push_back('\\');
      }
      escapedRegexPattern.push_back(c);
   }
   return escapedRegexPattern;
}
