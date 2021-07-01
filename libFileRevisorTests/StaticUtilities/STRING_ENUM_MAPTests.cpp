#include "pch.h"
#include "libFileRevisor/StaticUtilities/STRING_ENUM_MAP.h"

enum class TestingEnum
{
   A,
   B,
   C,
   MaxValue
};

BEGIN_STRING_ENUM_MAP(TestingEnum)
STRING_ENUM_MAP(TestingEnum, A)
STRING_ENUM_MAP(TestingEnum, B)
STRING_ENUM_MAP(TestingEnum, C)
STRING_ENUM_MAP(TestingEnum, MaxValue)
END_STRING_ENUM_MAP

INITIALIZE_STRING_ENUM_MAP(TestingEnum)

ostream& operator<<(ostream& os, TestingEnum testingEnum)
{
   os << ENUM_TO_STRING(TestingEnum, testingEnum);
   return os;
}

TESTS(STRING_ENUM_MAPTests)
AFACT(STRING_TO_ENUM_And_ENUM_TO_STRING_ReturnExpectedValues)
AFACT(OStreamInsertionOperator_WritesENUM_TO_STRINGToOStream)
EVIDENCE

TEST(STRING_TO_ENUM_And_ENUM_TO_STRING_ReturnExpectedValues)
{
   ARE_EQUAL(TestingEnum::A, STRING_TO_ENUM(TestingEnum, "A"));
   ARE_EQUAL("A", ENUM_TO_STRING(TestingEnum, TestingEnum::A));
}

TEST(OStreamInsertionOperator_WritesENUM_TO_STRINGToOStream)
{
   ostringstream oss;
   //
   oss << TestingEnum::A << ' ' << TestingEnum::B;
   //
   const string expectedString = String::ConcatStrings("A B");
   ARE_EQUAL(expectedString, oss.str());
}

RUN_TESTS(STRING_ENUM_MAPTests)
