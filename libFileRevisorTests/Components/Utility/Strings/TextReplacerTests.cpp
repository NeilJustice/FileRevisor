#include "pch.h"
#include "libFileRevisor/Components/Utility/Strings/TextReplacer.h"

TESTS(TextReplacerTests)
FACTS(ReplaceText_ReturnsReplacedString)
EVIDENCE

TextReplacer _textReplacer;

TEST4X4(ReplaceText_ReturnsReplacedString,
   string_view str, string_view substring, const string& replacement, const string& expectedReturnValue,
   "", "", "", "",
   "a", "b", "c", "a",
   "a", "a", "b", "b",
   "A", "a", "b", "A",
   "123|456", "3|4", "12_56", "1212_5656",
   R"(C:\Folder\Subfolder\File.ext)", R"(\Subfolder\)", R"(\Subfolder1\Subfolder2\)", R"(C:\Folder\Subfolder1\Subfolder2\File.ext)")
{
   const string replacedString = _textReplacer.ReplaceText(str, substring, replacement);
   ARE_EQUAL(expectedReturnValue, replacedString);
}

RUN_TESTS(TextReplacerTests)
