#include "pch.h"
#include "libFileRevisor/Components/Iteration/Counter/PredicateCounter.h"

TESTS(PredicateCounterTests)
AFACT(CountWhere_ZeroElementsMatchPredicate_Returns0)
AFACT(CountWhere_IsEvenPredicate_TwoElementsOutOfFiveAreEven_Returns2)
AFACT(CountWhere_IsOddPredicate_ThreeElementsOutOfFiveAreEven_Returns3)
EVIDENCE

PredicateCounter<int> _predicateCounter;

TEST(CountWhere_ZeroElementsMatchPredicate_Returns0)
{
   const vector<int> elements = { 1, 3, 5 };
   const auto isEvenPredicate = [](const int& element)
   {
      return element % 2 == 0;
   };
   //
   const size_t numberOfEvenElements = _predicateCounter.CountWhere(elements, isEvenPredicate);
   //
   IS_ZERO(numberOfEvenElements);
}

TEST(CountWhere_IsEvenPredicate_TwoElementsOutOfFiveAreEven_Returns2)
{
   const vector<int> elements = { 1, 2, 3, 4, 5 };
   const auto isEvenPredicate = [](const int& element)
   {
      return element % 2 == 0;
   };
   //
   const size_t numberOfEvenElements = _predicateCounter.CountWhere(elements, isEvenPredicate);
   //
   ARE_EQUAL(2, numberOfEvenElements);
}

TEST(CountWhere_IsOddPredicate_ThreeElementsOutOfFiveAreEven_Returns3)
{
   const vector<int> elements = { 1, 2, 3, 4, 5 };
   const auto isOddPredicate = [](const int& element)
   {
      return element % 2 == 1;
   };
   //
   const size_t numberOfOddElements = _predicateCounter.CountWhere(elements, isOddPredicate);
   //
   ARE_EQUAL(3, numberOfOddElements);
}

RUN_TESTS(PredicateCounterTests)
