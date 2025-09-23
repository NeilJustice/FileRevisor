#pragma once

template<typename ClassType, typename ElementType, typename TransformedElementType>
class OneArgMemberFunctionTransformer
{
public:
   virtual ~OneArgMemberFunctionTransformer() = default;

   using ConstMemberTransformFunctionType = TransformedElementType(ClassType::*)(const ElementType&) const;

   virtual vector<TransformedElementType> Transform(
      const vector<ElementType>& elements,
      const ClassType* classInstance,
      ConstMemberTransformFunctionType transformFunction) const
   {
      vector<TransformedElementType> transformedElements;
      transformedElements.reserve(elements.size());
      for (const ElementType& element : elements)
      {
         TransformedElementType transformedElement = (classInstance->*transformFunction)(element);
         transformedElements.emplace_back(std::move(transformedElement));
      }
      return transformedElements;
   }
};
