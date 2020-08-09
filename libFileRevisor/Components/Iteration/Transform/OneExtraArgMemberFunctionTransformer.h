#pragma once

template<typename ClassType, typename ElementType, typename TransformedElementType, typename ExtraArgType>
class OneExtraArgMemberFunctionTransformer
{
public:
   virtual vector<TransformedElementType> Transform(
      const vector<ElementType>& elements,
      const ClassType* classInstance,
      TransformedElementType(ClassType::* transformer)(const ElementType&, ExtraArgType) const,
      ExtraArgType extraArg) const
   {
      vector<TransformedElementType> transformedElements;
      transformedElements.reserve(elements.size());
      for (const ElementType& element : elements)
      {
         const TransformedElementType transformedElement = (classInstance->*transformer)(element, extraArg);
         transformedElements.push_back(transformedElement);
      }
      return transformedElements;
   }

   virtual ~OneExtraArgMemberFunctionTransformer() = default;
};
