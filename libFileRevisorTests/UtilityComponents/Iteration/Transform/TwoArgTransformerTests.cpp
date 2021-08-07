#include "pch.h"
#include "libFileRevisor/UtilityComponents/Iteration/Transform/TwoArgTransformer.h"

template<
   template<typename...>
   class TransformedContainerType, typename TransformedElementType,
   template<typename...>
   class SourceContainerType, typename SourceElementType,
   typename Arg2Type>
TEMPLATE_TESTS(TwoArgTransformerTests, TransformedContainerType, TransformedElementType, SourceContainerType, SourceElementType, Arg2Type)
AFACT(Transform_EmptyRange_DoesNothing)
AFACT(Transform_OneItemRange_CallsTransformerOnce)
AFACT(Transform_TwoItemRange_CallsTransformerTwice)
EVIDENCE

using TransformerType = TwoArgTransformer<
   TransformedContainerType<TransformedElementType>,
   SourceContainerType<SourceElementType>,
   TransformedElementType(*)(size_t, SourceElementType, Arg2Type),
   Arg2Type>;
TransformerType _transformer;

CLEANUP
{
   s_transformFunctionCalls.clear();
}

static vector<tuple<size_t, SourceElementType, Arg2Type>> s_transformFunctionCalls;

static TransformedElementType TransformFunction(size_t index, SourceElementType sourceElement, Arg2Type metadata)
{
   s_transformFunctionCalls.emplace_back(index, sourceElement, metadata);
   const TransformedElementType transformedElement = sourceElement + static_cast<TransformedElementType>(1);
   return transformedElement;
};

TEST(Transform_EmptyRange_DoesNothing)
{
   const SourceContainerType<SourceElementType> sourceElements;
   //
   const TransformedContainerType<TransformedElementType> transformedElements = _transformer.Transform(sourceElements, TransformFunction, 0);
   //
   IS_EMPTY(s_transformFunctionCalls);
   IS_EMPTY(transformedElements);
}

TEST(Transform_OneItemRange_CallsTransformerOnce)
{
   const SourceContainerType<SourceElementType> source = { 1 };
   //
   const TransformedContainerType<TransformedElementType> transformedElements = _transformer.Transform(source, TransformFunction, 10);
   //
   const vector<tuple<size_t, SourceElementType, Arg2Type>> expectedTransformFunctionCalls =
   {
      {0, SourceElementType(1), 10}
   };
   VECTORS_ARE_EQUAL(expectedTransformFunctionCalls, s_transformFunctionCalls);

   const TransformedContainerType<TransformedElementType> expectedTransformedElements = { 2 };
   ARE_EQUAL(expectedTransformedElements, transformedElements);
}

TEST(Transform_TwoItemRange_CallsTransformerTwice)
{
   const TwoArgTransformer<
      TransformedContainerType<TransformedElementType>,
      SourceContainerType<SourceElementType>,
      decltype(TransformFunction),
      Arg2Type> twoArgTransformer;
   SourceContainerType<SourceElementType> sourceElements = { 1, 2 };
   //
   const TransformedContainerType<TransformedElementType> transformedElements
      = twoArgTransformer.Transform(sourceElements, TransformFunction, 20);
   //
   const vector<tuple<size_t, SourceElementType, Arg2Type>> expectedTransformFunctionCalls =
   {
      {0, SourceElementType{1}, 20},
      {1, SourceElementType{2}, 20}
   };
   VECTORS_ARE_EQUAL(expectedTransformFunctionCalls, s_transformFunctionCalls);

   const TransformedContainerType<TransformedElementType> expectedTransformedElements = { 2, 3 };
   ARE_EQUAL(expectedTransformedElements, transformedElements);
}

RUN_TEMPLATE_TESTS(TwoArgTransformerTests, vector, int, vector, int, int)
THEN_RUN_TEMPLATE_TESTS(TwoArgTransformerTests, vector, double, vector, int, unsigned)

template<
   template<typename...>
   class TransformedContainerType, typename TransformedElementType,
   template<typename...>
   class SourceContainerType,
   typename SourceElementType, typename Arg2Type>
vector<tuple<size_t, SourceElementType, Arg2Type>> TwoArgTransformerTests<
   TransformedContainerType, TransformedElementType, SourceContainerType, SourceElementType, Arg2Type>::s_transformFunctionCalls;
