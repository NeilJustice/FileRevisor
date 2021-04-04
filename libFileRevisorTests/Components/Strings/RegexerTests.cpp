#include "pch.h"
#include "libFileRevisor/Components/Strings/Regexer.h"

TESTS(RegexerTests)
FACTS(RegexReplace_ReturnsRegexReplacedString)
EVIDENCE

const Regexer _regexer;

TEST4X4(RegexReplace_ReturnsRegexReplacedString,
   string_view str, string_view matchPattern, string_view replacementPattern, const string& expectedReturnValue,
   "", "", "", "",
   "a", "b", "c", "a",
   "a", "a", "b", "b",
   "A", "a", "b", "A",
   "123 -> 456", "\\d\\d\\d -> ", "|", "|456")
{
   const string regexReplacedString = _regexer.Replace(str, matchPattern, replacementPattern);
   ARE_EQUAL(expectedReturnValue, regexReplacedString);
}

RUN_TESTS(RegexerTests)
