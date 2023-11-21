#include "pch.h"
#include "libFileRevisor/Components/Utility/Strings/RegexReplacer.h"

string RegexReplacer::RegexReplace(string_view str, string_view matchPattern, string_view replacementPattern) const
{
   const regex regexMatchingPattern(matchPattern.data());
   string regexReplacedString = regex_replace(str.data(), regexMatchingPattern, replacementPattern.data());
   return regexReplacedString;
}
