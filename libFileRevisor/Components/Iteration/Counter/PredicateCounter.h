#pragma once

template<typename ContainerType, typename T>
class PredicateCounter
{
public:
   // Rule of 5 to satisfy clang-tidy hicpp-special-member-functions
   PredicateCounter() = default;
   PredicateCounter(const PredicateCounter&) = default;
   PredicateCounter& operator=(const PredicateCounter&) = default;
   PredicateCounter(PredicateCounter&&) noexcept = default;
   PredicateCounter& operator=(PredicateCounter&&) noexcept = default;

   using PredicateFunctionPointerType = bool(*)(const T&);

   virtual size_t CountWhere(const ContainerType& elements, PredicateFunctionPointerType predicateFunction) const
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
