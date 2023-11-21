#include "pch.h"
#include "libFileRevisor/Components/Utility/Strings/TextReplacer.h"

TESTS(TextReplacerTests)
AFACT(ReplaceText_SubstringIsEmptyString_ThrowsInvalidArgument)
FACTS(ReplaceText_ReturnsReplacedString)
EVIDENCE

TextReplacer _textReplacer;

TEST(ReplaceText_SubstringIsEmptyString_ThrowsInvalidArgument)
{
   const string str = ZenUnit::Random<string>();
   const string replacement = ZenUnit::Random<string>();
   //
   THROWS_EXCEPTION(_textReplacer.ReplaceText(str, "", replacement),
      invalid_argument,
      "Utils::ReplaceAllSubstrings(string_view str, string_view substring, string_view replacement) called with empty substring");
}

TEST4X4(ReplaceText_ReturnsReplacedString,
   string_view str, string_view substring, string_view replacement, const string& expectedReturnValue,
   " a", "a", "b", " b",
   "1", "\\d", "2", "1",
   "a a a", "a", "gg", "gg gg gg",
   "A", "a", "b", "A")
{
   const string replacedString = _textReplacer.ReplaceText(str, substring, replacement);
   ARE_EQUAL(expectedReturnValue, replacedString);
}

RUN_TESTS(TextReplacerTests)
