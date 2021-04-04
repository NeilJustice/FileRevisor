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
AFACT(Test)
EVIDENCE

TEST(Test)
{
   ARE_EQUAL(TestingEnum::A, STRING_TO_ENUM(TestingEnum, "A"));
   ARE_EQUAL("A", ENUM_TO_STRING(TestingEnum, TestingEnum::A));
}

RUN_TESTS(STRING_ENUM_MAPTests)
