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
      const SourceContainerType& sourceElements, TransformFunctionType transformFunction, Arg2Type arg2) const
   {
      TransformedContainerType transformedElements;
      transformedElements.reserve(sourceElements.size());
      const auto endSourceIter = sourceElements.end();
      size_t index = 0;
      for (auto iter = sourceElements.begin(); iter != endSourceIter; ++iter)
      {
         const auto& sourceElement = *iter;
         transformedElements.push_back(transformFunction(index++, sourceElement, arg2));
      }
      return transformedElements;
   }

   virtual ~TwoArgTransformer() = default;
};
