#include "pch.h"
#include "libFileRevisor/Components/Iteration/Math/OneArgMemberFunctionAccumulator.h"

template<typename SumType, typename ElementType>
TEMPLATE_TESTS(OneArgMemberFunctionAccumulatorTests, SumType, ElementType)
AFACT(SumElementsWithFunction_ElementsAreEmpty_DoesNotCallMemberFunction_ReturnsDefaultSumType)
AFACT(SumElementsWithFunction_CallsMemberFunctionElementsNumberOfTimes_ReturnsSumOfFunctionReturnValues)
EVIDENCE

class AccumulatorTestClass
{
public:
   mutable size_t _numberOfFunctionCalls = 0;
   mutable vector<ElementType> _elementArgs;
   mutable SumType _functionReturnValue = {};

   SumType SumationFunction(const ElementType& element) const
   {
      ++_numberOfFunctionCalls;
      _elementArgs.push_back(element);
      return _functionReturnValue;
   }
};

OneArgMemberFunctionAccumulator<AccumulatorTestClass, SumType, ElementType> _oneArgMemberFunctionAccumulator;
AccumulatorTestClass _sumatorTestClass;

TEST(SumElementsWithFunction_ElementsAreEmpty_DoesNotCallMemberFunction_ReturnsDefaultSumType)
{
   const vector<ElementType> emptyElements;
   //
   const SumType sum = _oneArgMemberFunctionAccumulator.SumElementsWithFunction(
      emptyElements, &_sumatorTestClass, &AccumulatorTestClass::SumationFunction);
   //
   ARE_EQUAL(0, _sumatorTestClass._numberOfFunctionCalls);
   IS_EMPTY(_sumatorTestClass._elementArgs);
   const SumType expectedSum{};
   ARE_EQUAL(expectedSum, sum);
}

TEST(SumElementsWithFunction_CallsMemberFunctionElementsNumberOfTimes_ReturnsSumOfFunctionReturnValues)
{
   _sumatorTestClass._functionReturnValue = ZenUnit::RandomBetween<SumType>(-100, 100);
   const vector<ElementType> elements = { ZenUnit::Random<ElementType>(), ZenUnit::Random<ElementType>() };
   //
   const SumType sum = _oneArgMemberFunctionAccumulator.SumElementsWithFunction(
      elements, &_sumatorTestClass, &AccumulatorTestClass::SumationFunction);
   //
   ARE_EQUAL(2, _sumatorTestClass._numberOfFunctionCalls);

   const vector<ElementType> expectedElementArgs =
   {
      elements[0],
      elements[1]
   };
   VECTORS_ARE_EQUAL(expectedElementArgs, _sumatorTestClass._elementArgs);

   const SumType expectedSum = _sumatorTestClass._functionReturnValue + _sumatorTestClass._functionReturnValue;
   ARE_EQUAL(expectedSum, sum);
}

RUN_TEMPLATE_TESTS(OneArgMemberFunctionAccumulatorTests, long long, int)
THEN_RUN_TEMPLATE_TESTS(OneArgMemberFunctionAccumulatorTests, long long, short)
