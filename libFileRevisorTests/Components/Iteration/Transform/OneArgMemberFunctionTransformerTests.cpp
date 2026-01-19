#include "pch.h"
#include "libFileRevisor/Components/Iteration/Transform/OneArgMemberFunctionTransformer.h"

template<typename ElementType, typename TransformedElementType>
TEMPLATE_TESTS(OneArgMemberFunctionTransformerTests, ElementType, TransformedElementType)
AFACT(Transform_ElementsEmpty_DoesNotCallTransformFunction_ReturnsEmptyVector)
AFACT(Transform_TwoElements_CallsTransformFunctionOnEachElement_ReturnsTransformedElements)
EVIDENCE

class Class
{
public:
   mutable vector<ElementType> calls;

   [[noreturn]] TransformedElementType ConstMemberFunctionThatThrowsIfCalled(const ElementType& /*element*/) const
   {
      throw runtime_error("Expected call to ConstMemberFunctionThatThrows()");
   }

   TransformedElementType ConstMemberFunction(const ElementType& element) const
   {
      calls.emplace_back(element);
      const TransformedElementType transformedElement = static_cast<TransformedElementType>(element + ElementType{ 1 });
      return transformedElement;
   }
};

OneArgMemberFunctionTransformer<Class, ElementType, TransformedElementType> _oneArgMemberFunctionTransformer;
Class _classInstance;

TEST(Transform_ElementsEmpty_DoesNotCallTransformFunction_ReturnsEmptyVector)
{
   const vector<ElementType> emptyElements;
   //
   const vector<TransformedElementType> transformedElements = _oneArgMemberFunctionTransformer.Transform(
      emptyElements,
      &_classInstance, &Class::ConstMemberFunctionThatThrowsIfCalled);
   //
   IS_EMPTY(transformedElements);

   THROWS_EXCEPTION(_classInstance.ConstMemberFunctionThatThrowsIfCalled(ElementType{}), runtime_error,
      "Expected call to ConstMemberFunctionThatThrows()"); // 100% code coverage
}

TEST(Transform_TwoElements_CallsTransformFunctionOnEachElement_ReturnsTransformedElements)
{
   const vector<ElementType> elements =
   {
      ElementType{1},
      ElementType{2}
   };
   //
   const vector<TransformedElementType> transformedElements = _oneArgMemberFunctionTransformer.Transform(
      elements,
      &_classInstance, &Class::ConstMemberFunction);
   //
   vector<ElementType> expectedCalls =
   {
      ElementType{1},
      ElementType{2}
   };
   VECTORS_ARE_EQUAL(expectedCalls, _classInstance.calls);

   vector<TransformedElementType> expectedTransformedElements =
   {
      static_cast<TransformedElementType>(elements[0] + 1),
      static_cast<TransformedElementType>(elements[1] + 1)
   };
   VECTORS_ARE_EQUAL(expectedTransformedElements, transformedElements);
}

RUN_TEMPLATE_TESTS(OneArgMemberFunctionTransformerTests, int, int)
THEN_RUN_TEMPLATE_TESTS(OneArgMemberFunctionTransformerTests, unsigned long long, int)
