#pragma once

template<
   typename ClassType,
   typename SumType,
   typename ElementType,
   typename ExtraArgType>
   class OneExtraArgMemberFunctionAccumulator
{
public:
   virtual ~OneExtraArgMemberFunctionAccumulator() = default;

   virtual SumType SumElementsWithFunction(
      const vector<ElementType>& elements,
      const ClassType* classInstance,
      SumType(ClassType::*sumationFunction)(const ElementType&, const ExtraArgType&) const,
      const ExtraArgType& extraArg) const
   {
      SumType sum{};
      for (const ElementType& element : elements)
      {
         const SumType functionReturnValue = (classInstance->*sumationFunction)(element, extraArg);
         sum += functionReturnValue;
      }
      return sum;
   }
};
