#pragma once
#include "libFileRevisor/Components/Iteration/Transform/OneExtraArgMemberFunctionTransformer.h"

template<typename ClassType, typename ElementType, typename TransformedElementType, typename ExtraArgType>
class OneExtraArgMemberFunctionTransformerMock : public Metal::Mock<OneExtraArgMemberFunctionTransformer<ClassType, ElementType, TransformedElementType, ExtraArgType>>
{
public:
   using MemberFunctionType = TransformedElementType(ClassType::*)(const ElementType&, ExtraArgType) const;
   METALMOCK_NONVOID4_CONST(vector<TransformedElementType>, Transform, const vector<ElementType>&, const ClassType*, MemberFunctionType, ExtraArgType)
};
