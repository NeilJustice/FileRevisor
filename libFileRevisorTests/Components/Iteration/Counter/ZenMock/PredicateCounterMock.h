#pragma once
#include "libFileRevisor/Components/Iteration/Counter/PredicateCounter.h"

template<typename ContainerType, typename T>
class PredicateCounterMock : public Zen::Mock<PredicateCounter<ContainerType, T>>
{
public:
   using PredicateFunctionPointerType = bool(*)(const T&);
   ZENMOCK_NONVOID2_CONST(size_t, CountWhere, const ContainerType&, PredicateFunctionPointerType)
};
