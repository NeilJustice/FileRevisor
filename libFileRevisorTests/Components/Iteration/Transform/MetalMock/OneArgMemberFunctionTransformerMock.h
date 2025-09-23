#pragma once
#include "libFileRevisor/Components/Iteration/Transform/OneArgMemberFunctionTransformer.h"

template<typename ClassType, typename ElementType, typename TransformedElementType>
class OneArgMemberFunctionTransformerMock : public Metal::Mock<OneArgMemberFunctionTransformer<ClassType, ElementType, TransformedElementType>>
{
public:
   using ConstMemberTransformFunctionType = TransformedElementType(ClassType::*)(const ElementType&) const;

   METALMOCK_NONVOID3_CONST(vector<TransformedElementType>, Transform,
      const vector<ElementType>&,
      const ClassType*,
      ConstMemberTransformFunctionType)
};
