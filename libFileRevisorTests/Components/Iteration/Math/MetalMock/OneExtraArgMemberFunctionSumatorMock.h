#pragma once
#include "libFileRevisor/Components/Iteration/Math/OneExtraArgMemberFunctionSumator.h"

template<
   typename ClassType,
   typename SumType,
   template<typename _ElementType, typename...> typename ContainerType,
   typename ElementType,
   typename ExtraArgType>
   class OneExtraArgMemberFunctionSumatorMock : public Zen::Mock<OneExtraArgMemberFunctionSumator<ClassType, SumType, ContainerType, ElementType, ExtraArgType>>
{
public:
   using MemberFunctionType = SumType(ClassType::*)(const ElementType&, const ExtraArgType&) const;
   METALMOCK_NONVOID4_CONST(SumType, SumElementsWithFunction, const ClassType*, const ContainerType<ElementType>&, MemberFunctionType, const ExtraArgType&)
};
