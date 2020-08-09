#pragma once
#include "libFileRevisor/Components/Iteration/Transform/TwoArgTransformer.h"

template<
   typename TransformedContainerType,
   typename SourceContainerType,
   typename TransformFunctionType,
   typename Arg2Type>
   class TwoArgTransformerMock : public Metal::Mock<TwoArgTransformer<TransformedContainerType, SourceContainerType, TransformFunctionType, Arg2Type>>
{
public:
   METALMOCK_NONVOID3_CONST(TransformedContainerType, Transform, const SourceContainerType&, TransformFunctionType, Arg2Type)
};
