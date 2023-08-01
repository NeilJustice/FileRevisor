#pragma once

template<typename T>
class PredicateCounter
{
public:
   using PredicateFunctionPointerType = bool(*)(const T&);

   virtual size_t CountWhere(const vector<T>& elements, PredicateFunctionPointerType predicateFunction) const
   {
      size_t numberOfElementsMatchingPredicate = 0;
      const size_t numberOfElements = elements.size();
      for (size_t i = 0; i < numberOfElements; ++i)
      {
         const T& element = elements[i];
         const bool elementMatchesPredicate = predicateFunction(element);
         numberOfElementsMatchingPredicate += elementMatchesPredicate ? 1 : 0;
      }
      return numberOfElementsMatchingPredicate;
   }

   virtual ~PredicateCounter() = default;
};
