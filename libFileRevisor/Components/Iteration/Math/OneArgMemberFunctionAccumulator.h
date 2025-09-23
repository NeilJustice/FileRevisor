#pragma once

template<
   typename ClassType,
   typename SumType,
   typename ElementType>
   class OneArgMemberFunctionAccumulator
{
public:
   virtual ~OneArgMemberFunctionAccumulator() = default;

   using ConstMemberSumationFunctionType = SumType(ClassType::*)(const ElementType&) const;

   virtual SumType SumElementsWithFunction(
      const vector<ElementType>& elements,
      const ClassType* classInstance,
      ConstMemberSumationFunctionType constMemberSumationFunction) const
   {
      SumType sum{};
      for (const ElementType& element : elements)
      {
         const SumType functionReturnValue = (classInstance->*constMemberSumationFunction)(element);
         sum += functionReturnValue;
      }
      return sum;
   }
};
