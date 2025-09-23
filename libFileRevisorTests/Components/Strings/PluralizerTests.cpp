#include "pch.h"
#include "libFileRevisor/Components/Strings/Pluralizer.h"

TESTS(PluralizerTests)
AFACT(PotentiallyPluralizeWord_CountIs1_ReturnsSingularWord)
FACTS(PotentiallyPluralizeWord_CountIsNot1_ReturnsPluralWord)
EVIDENCE

Pluralizer p_pluralizer;

TEST(PotentiallyPluralizeWord_CountIs1_ReturnsSingularWord)
{
   const string singularWord = ZenUnit::Random<string>();
   const string pluralWord = ZenUnit::Random<string>();
   //
   const string potentiallyPluralizedWord = p_pluralizer.PotentiallyPluralizeWord(1, singularWord, pluralWord);
   //
   ARE_EQUAL(singularWord, potentiallyPluralizedWord);
}

TEST1X1(PotentiallyPluralizeWord_CountIsNot1_ReturnsPluralWord,
   size_t count,
   0ULL,
   2ULL,
   3ULL)
{
   const string singularWord = ZenUnit::Random<string>();
   const string pluralWord = ZenUnit::Random<string>();
   //
   const string potentiallyPluralizedWord = p_pluralizer.PotentiallyPluralizeWord(count, singularWord, pluralWord);
   //
   ARE_EQUAL(pluralWord, potentiallyPluralizedWord);
}

RUN_TESTS(PluralizerTests)
