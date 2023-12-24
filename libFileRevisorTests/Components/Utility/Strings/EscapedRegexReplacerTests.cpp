#include "pch.h"
#include "libFileRevisor/Components/Utility/Strings/EscapedRegexReplacer.h"

TESTS(EscapedRegexReplacerTests)
FACTS(ReplaceText_ReturnsReplacedString)
EVIDENCE

EscapedRegexReplacer _escapedRegexReplacer;

TEST4X4(ReplaceText_ReturnsReplacedString,
   string_view str, string_view matchPattern, string_view replacementPattern, const string& expectedReturnValue,
   "", "", "", "",
   "a", "b", "c", "a",
   "a", "a", "b", "b",
   "A", "a", "b", "A",
   "123 -> 456", "\\d\\d\\d -> ", "|", "|456")
{
   const string regexReplacedString = _escapedRegexReplacer.ReplaceText(str, matchPattern, replacementPattern);
   ARE_EQUAL(expectedReturnValue, regexReplacedString);
}

RUN_TESTS(EscapedRegexReplacerTests)
