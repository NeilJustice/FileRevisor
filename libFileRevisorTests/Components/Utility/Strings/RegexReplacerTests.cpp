#include "pch.h"
#include "libFileRevisor/Components/Utility/Strings/RegexReplacer.h"

TESTS(RegexReplacerTests)
FACTS(RegexReplace_ReturnsReplacedString)
EVIDENCE

RegexReplacer _regexReplacer;

TEST4X4(RegexReplace_ReturnsReplacedString,
   string_view str, string_view matchPattern, string_view replacementPattern, const string& expectedReturnValue,
   "", "", "", "",
   "a", "b", "c", "a",
   "a", "a", "b", "b",
   "A", "a", "b", "A",
   "123 -> 456", "\\d\\d\\d -> ", "|", "|456")
{
   const string regexReplacedString = _regexReplacer.RegexReplace(str, matchPattern, replacementPattern);
   ARE_EQUAL(expectedReturnValue, regexReplacedString);
}

RUN_TESTS(RegexReplacerTests)
