#pragma once

template<
   typename ClassType,
   typename SumType,
   template<typename _ElementType, typename...> typename ContainerType,
   typename ElementType,
   typename ExtraArgType>
   class OneExtraArgMemberFunctionSumator
{
public:
   virtual SumType SumElementsWithFunction(
      const ClassType* classInstance,
      const ContainerType<ElementType>& elements,
      SumType(ClassType::* sumationFunction)(const ElementType&, const ExtraArgType&) const,
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

   virtual ~OneExtraArgMemberFunctionSumator() = default;
};
