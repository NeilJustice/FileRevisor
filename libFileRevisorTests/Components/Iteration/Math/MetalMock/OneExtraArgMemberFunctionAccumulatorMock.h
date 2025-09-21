#pragma once
#include "libFileRevisor/Components/Iteration/Math/OneExtraArgMemberFunctionAccumulator.h"

template<
   typename ClassType,
   typename SumType,
   typename ElementType,
   typename ExtraArgType>
class OneExtraArgMemberFunctionAccumulatorMock : public Metal::Mock<OneExtraArgMemberFunctionAccumulator<ClassType, SumType, ElementType, ExtraArgType>>
{
public:
   using MemberFunctionType = SumType(ClassType::*)(const ElementType&, const ExtraArgType&) const;
   METALMOCK_NONVOID4_CONST(SumType, SumElementsWithFunction, const ClassType*, const vector<ElementType>&, MemberFunctionType, const ExtraArgType&)
};
