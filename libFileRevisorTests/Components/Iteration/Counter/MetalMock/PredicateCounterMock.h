#pragma once
#include "libFileRevisor/Components/Iteration/Counter/PredicateCounter.h"

template<typename T>
class PredicateCounterMock : public Metal::Mock<PredicateCounter<T>>
{
public:
   using PredicateFunctionPointerType = bool(*)(const T&);
   METALMOCK_NONVOID2_CONST(size_t, CountWhere, const vector<T>&, PredicateFunctionPointerType)
};
