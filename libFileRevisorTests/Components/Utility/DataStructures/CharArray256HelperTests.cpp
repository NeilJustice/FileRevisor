#include "pch.h"
#include "libFileRevisor/Components/Utility/DataStructures/CharArray256Helper.h"

TESTS(CharArray256HelperTests)
AFACT(ArrayContains0_MaximumNumberOfElementsToCompareIs0_ReturnsFalse)
AFACT(ArrayContains0_MaximumNumberOfElementsToCompareIs1_Char0IsNot0_ReturnsFalse)
AFACT(ArrayContains0_MaximumNumberOfElementsToCompareIs1_Char0Is0_ReturnsTrue)
AFACT(ArrayContains0_MaximumNumberOfElementsToCompareIs2_Char0IsNot0_Char1IsNot0_ReturnsFalse)
AFACT(ArrayContains0_MaximumNumberOfElementsToCompareIs2_Char0IsNot0_Char1Is0_ReturnsTrue)
AFACT(ArrayContains0_MaximumNumberOfElementsToCompareIs64_Chars0Through63AreNot0_ReturnsFalse)
AFACT(ArrayContains0_MaximumNumberOfElementsToCompareIs64_Chars0Through62IsNot0_Char63Is0_ReturnsTrue)
EVIDENCE

CharArray256Helper _charArray256Helper;

TEST(ArrayContains0_MaximumNumberOfElementsToCompareIs0_ReturnsFalse)
{
   const array<char, 256> chars = ZenUnit::RandomStdArray<char, 256>();
   //
   const bool arrayContainsElement = _charArray256Helper.ArrayContains0(chars, 0);
   //
   IS_FALSE(arrayContainsElement);
}

TEST(ArrayContains0_MaximumNumberOfElementsToCompareIs1_Char0IsNot0_ReturnsFalse)
{
   array<char, 256> chars = ZenUnit::RandomStdArray<char, 256>();
   chars[0] = ZenUnit::RandomNotEqualTo<char>(0);
   //
   const bool arrayContains0 = _charArray256Helper.ArrayContains0(chars, 1);
   //
   IS_FALSE(arrayContains0);
}

TEST(ArrayContains0_MaximumNumberOfElementsToCompareIs1_Char0Is0_ReturnsTrue)
{
   array<char, 256> chars = ZenUnit::RandomStdArray<char, 256>();
   chars[0] = 0;
   //
   const bool arrayContains0 = _charArray256Helper.ArrayContains0(chars, 1);
   //
   IS_TRUE(arrayContains0);
}

TEST(ArrayContains0_MaximumNumberOfElementsToCompareIs2_Char0IsNot0_Char1IsNot0_ReturnsFalse)
{
   array<char, 256> chars = ZenUnit::RandomStdArray<char, 256>();
   chars[0] = ZenUnit::RandomNotEqualTo<char>(0);
   chars[1] = ZenUnit::RandomNotEqualTo<char>(0);
   //
   const bool arrayContains0 = _charArray256Helper.ArrayContains0(chars, 2);
   //
   IS_FALSE(arrayContains0);
}

TEST(ArrayContains0_MaximumNumberOfElementsToCompareIs2_Char0IsNot0_Char1Is0_ReturnsTrue)
{
   array<char, 256> chars = ZenUnit::RandomStdArray<char, 256>();
   chars[0] = ZenUnit::RandomNotEqualTo<char>(0);
   chars[1] = 0;
   //
   const bool arrayContains0 = _charArray256Helper.ArrayContains0(chars, 2);
   //
   IS_TRUE(arrayContains0);
}

TEST(ArrayContains0_MaximumNumberOfElementsToCompareIs64_Chars0Through63AreNot0_ReturnsFalse)
{
   array<char, 256> chars{};
   std::iota(chars.begin(), chars.end(), static_cast<char>(1));
   //
   const bool arrayContains0 = _charArray256Helper.ArrayContains0(chars, 64);
   //
   IS_FALSE(arrayContains0);
}

TEST(ArrayContains0_MaximumNumberOfElementsToCompareIs64_Chars0Through62IsNot0_Char63Is0_ReturnsTrue)
{
   array<char, 256> chars{};
   std::iota(chars.begin(), chars.end(), static_cast<char>(1));
   chars[63] = 0;
   //
   const bool arrayContains0 = _charArray256Helper.ArrayContains0(chars, 64);
   //
   IS_TRUE(arrayContains0);
}

RUN_TESTS(CharArray256HelperTests)
