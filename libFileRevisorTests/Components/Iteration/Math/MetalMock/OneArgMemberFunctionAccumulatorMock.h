#pragma once
#include "libFileRevisor/Components/Iteration/Math/OneArgMemberFunctionAccumulator.h"

template<
   typename ClassType,
   typename SumType,
   typename ElementType>
class OneArgMemberFunctionAccumulatorMock : public Metal::Mock<OneArgMemberFunctionAccumulator<ClassType, SumType, ElementType>>
{
public:
   using ConstMemberSumationFunctionType = SumType(ClassType::*)(const ElementType&) const;

   METALMOCK_NONVOID3_CONST(SumType, SumElementsWithFunction,
      const vector<ElementType>&,
      const ClassType*,
      ConstMemberSumationFunctionType)
};
