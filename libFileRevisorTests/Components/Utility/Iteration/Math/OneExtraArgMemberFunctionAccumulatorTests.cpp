#include "pch.h"
#include "libFileRevisor/Components/Utility/Iteration/Math/OneExtraArgMemberFunctionAccumulator.h"

template<
   typename SumType,
   typename ElementType,
   typename ExtraArgType>
TEMPLATE_TESTS(OneExtraArgMemberFunctionAccumulatorTests, SumType, ElementType, ExtraArgType)
AFACT(SumElementsWithFunction_ElementsAreEmpty_DoesNotCallMemberFunction_ReturnsDefaultSumType)
AFACT(SumElementsWithFunction_CallsMemberFunctionElementsNumberOfTimes_ReturnsSumOfFunctionReturnValues)
EVIDENCE

class AccumulatorTestClass
{
public:
   mutable size_t _numberOfFunctionCalls = 0;
   mutable vector<ElementType> _elementArgs;
   mutable vector<ExtraArgType> _extraArgArgs;
   mutable SumType _functionReturnValue = {};

   SumType SumationFunction(const ElementType& element, const ExtraArgType& extraArg) const
   {
      ++_numberOfFunctionCalls;
      _elementArgs.push_back(element);
      _extraArgArgs.push_back(extraArg);
      return _functionReturnValue;
   }
};

OneExtraArgMemberFunctionAccumulator<AccumulatorTestClass, SumType, ElementType, ExtraArgType> _oneExtraArgMemberFunctionAccumulator;
AccumulatorTestClass _sumatorTestClass;

TEST(SumElementsWithFunction_ElementsAreEmpty_DoesNotCallMemberFunction_ReturnsDefaultSumType)
{
   const vector<ElementType> emptyElements;
   const ExtraArgType extraArg = ZenUnit::Random<ExtraArgType>();
   //
   const SumType sum = _oneExtraArgMemberFunctionAccumulator.SumElementsWithFunction(
      &_sumatorTestClass, emptyElements, &AccumulatorTestClass::SumationFunction, extraArg);
   //
   ARE_EQUAL(0, _sumatorTestClass._numberOfFunctionCalls);
   IS_EMPTY(_sumatorTestClass._elementArgs);
   IS_EMPTY(_sumatorTestClass._extraArgArgs);
   const SumType expectedSum{};
   ARE_EQUAL(expectedSum, sum);
}

TEST(SumElementsWithFunction_CallsMemberFunctionElementsNumberOfTimes_ReturnsSumOfFunctionReturnValues)
{
   _sumatorTestClass._functionReturnValue = ZenUnit::RandomBetween<SumType>(-100, 100);

   const vector<ElementType> elements = { ZenUnit::Random<ElementType>(), ZenUnit::Random<ElementType>() };
   const ExtraArgType extraArg = ZenUnit::Random<ExtraArgType>();
   //
   const SumType sum = _oneExtraArgMemberFunctionAccumulator.SumElementsWithFunction(
      &_sumatorTestClass, elements, &AccumulatorTestClass::SumationFunction, extraArg);
   //
   ARE_EQUAL(2, _sumatorTestClass._numberOfFunctionCalls);

   const vector<ExtraArgType> expectedElementArgs = { elements[0], elements[1] };
   VECTORS_ARE_EQUAL(expectedElementArgs, _sumatorTestClass._elementArgs);

   const vector<ExtraArgType> expectedExtraArgArgs = { extraArg, extraArg };
   VECTORS_ARE_EQUAL(expectedExtraArgArgs, _sumatorTestClass._extraArgArgs);

   const SumType expectedSum = _sumatorTestClass._functionReturnValue + _sumatorTestClass._functionReturnValue;
   ARE_EQUAL(expectedSum, sum);
}

RUN_TEMPLATE_TESTS(OneExtraArgMemberFunctionAccumulatorTests, long long, int, int)
THEN_RUN_TEMPLATE_TESTS(OneExtraArgMemberFunctionAccumulatorTests, long long, short, short)
