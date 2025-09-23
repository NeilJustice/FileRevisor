#pragma once

template<typename ClassType, typename ElementType, typename TransformedElementType, typename ExtraArgType>
class OneExtraArgMemberFunctionTransformer
{
public:
   virtual ~OneExtraArgMemberFunctionTransformer() = default;

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
         TransformedElementType transformedElement = (classInstance->*transformer)(element, extraArg);
         transformedElements.emplace_back(std::move(transformedElement));
      }
      return transformedElements;
   }
};
