#include "pch.h"
#include "libFileRevisor/StaticUtilities/StringUtil.h"

TESTS(StringUtilTests)
AFACT(ConcatStrings_ReturnsStringsConcatenatedIntoAString)
AFACT(ConcatValues_ReturnsValuesConcatenatedIntoAString)
FACTS(ContainsSubstring_ReturnsTrueIfStringContainsSubstring)
FACTS(CaseInsensitiveContainsSubstring_ReturnsTrueIfStringCaseInsensitiveContainsSubstring)
FACTS(ReplaceText_ReturnsReplacedString)
EVIDENCE

struct UserType
{
   int x;
   explicit UserType(int x) : x(x) {}

   friend ostream& operator<<(ostream& os, const UserType& userType)
   {
      os << userType.x;
      return os;
   }
};

TEST(ConcatStrings_ReturnsStringsConcatenatedIntoAString)
{
   ARE_EQUAL("", String::ConcatStrings(""));
   ARE_EQUAL("1", String::ConcatStrings("1"));
   ARE_EQUAL("12", String::ConcatStrings("1", "2"));
   ARE_EQUAL("123", String::ConcatStrings("1", "2", "3"));

   const string str1 = "str1";
   string_view str2 = "str2";
   const char* const str3 = "str3";
   ARE_EQUAL("str1str2str3", String::ConcatStrings(str1, str2, str3));
}

TEST(ConcatValues_ReturnsValuesConcatenatedIntoAString)
{
   ARE_EQUAL("", String::ConcatValues(""));
   ARE_EQUAL("1", String::ConcatValues("1"));
   ARE_EQUAL("12", String::ConcatValues("1", "2"));
   ARE_EQUAL("123", String::ConcatValues("1", "2", "3"));

   const UserType userType1(1);
   const string str = "hello";
   const UserType userType3(3);
   ARE_EQUAL("1hello3", String::ConcatValues(userType1, str, userType3));
}

TEST3X3(ContainsSubstring_ReturnsTrueIfStringContainsSubstring,
   bool expectedReturnValue, string_view str, string_view substring,
   true, "", "",
   true, " ", "",
   true, " ", " ",
   true, "  ", " ",
   false, "", " ",
   true, "abc", "abc",
   false, "Abc", "abc",
   true, "abc", "ab",
   true, "abc", "bc",
   true, "abc def ghi", "c d",
   true, "a\r\n_bc_", "bc")
{
   ARE_EQUAL(expectedReturnValue, String::ContainsSubstring(str, substring));
}

TEST3X3(CaseInsensitiveContainsSubstring_ReturnsTrueIfStringCaseInsensitiveContainsSubstring,
   bool expectedReturnValue, string_view str, string_view substring,
   true, "", "",
   true, " ", "",
   true, " ", " ",
   true, "  ", " ",
   false, "", " ",
   true, "abc", "abc",
   true, "ABC", "abc",
   true, "abc", "ABC",
   true, "abc", "ab",
   true, "abc", "bc",
   true, "abc def ghi", "c d",
   true, "a\r\n_bc_", "bc")
{
   ARE_EQUAL(expectedReturnValue, String::CaseInsensitiveContainsSubstring(str, substring));
}

TEST4X4(ReplaceText_ReturnsReplacedString,
   string_view str, string_view pattern, string_view replacement, string_view expectedReturnValue,
   "", "", "", "",
   "a", "b", "c", "a",
   "a", "a", "b", "b",
   "A", "a", "b", "A",
   "123 -> 456", "\\d\\d\\d -> ", "|", "|456")
{
   const string replacedString = String::ReplaceText(str, pattern, replacement);
   ARE_EQUAL(expectedReturnValue, replacedString);
}

RUN_TESTS(StringUtilTests)
