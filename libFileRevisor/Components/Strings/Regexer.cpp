#include "pch.h"
#include "libFileRevisor/Components/Strings/Regexer.h"

string Regexer::Replace(string_view str, string_view matchPattern, string_view replacementPattern) const
{
   const regex regexMatchingPattern(matchPattern.data());
   const string regexReplacedString = regex_replace(str.data(), regexMatchingPattern, replacementPattern.data());
   return regexReplacedString;
}