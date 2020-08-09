#pragma once

template<
   typename TransformedContainerType,
   typename SourceContainerType,
   typename TransformFunctionType,
   typename Arg2Type>
   class TwoArgTransformer
{
public:
   virtual TransformedContainerType Transform(
      const SourceContainerType& sourceContainer, TransformFunctionType transformFunction, Arg2Type arg2) const
   {
      TransformedContainerType transformedElements;
      transformedElements.reserve(sourceContainer.size());
      const auto endSourceIter = sourceContainer.end();
      size_t index = 0;
      for (auto iter = sourceContainer.begin(); iter != endSourceIter; ++iter)
      {
         const auto& sourceElement = *iter;
         transformedElements.push_back(transformFunction(index++, sourceElement, arg2));
      }
      return transformedElements;
   }

   virtual ~TwoArgTransformer() = default;
};
