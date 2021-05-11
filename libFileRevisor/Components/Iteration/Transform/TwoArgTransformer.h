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
      TransformedContainerType transformedElements{};
      transformedElements.reserve(sourceElements.size());
      const auto endSourceIter = sourceElements.end();
      size_t index = 0;
      for (auto iter = sourceElements.begin(); iter != endSourceIter; ++iter)
      {
         const auto& sourceElement = *iter;
         auto transformedElement = transformFunction(index++, sourceElement, arg2);
         transformedElements.emplace_back(std::move(transformedElement));
      }
      return transformedElements;
   }

   virtual ~TwoArgTransformer() = default;
};
