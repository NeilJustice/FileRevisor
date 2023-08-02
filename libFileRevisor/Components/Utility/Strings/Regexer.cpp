#include "pch.h"
#include "libFileRevisor/Components/Utility/Strings/Regexer.h"

string Regexer::Replace(string_view str, string_view matchPattern, string_view replacementPattern) const
{
   const regex regexMatchingPattern(matchPattern.data());
   string regexReplacedString = regex_replace(str.data(), regexMatchingPattern, replacementPattern.data());
   return regexReplacedString;
}
