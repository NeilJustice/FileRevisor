#pragma once
#include "libFileRevisor/UtilityComponents/Iteration/Counter/PredicateCounter.h"

template<typename ContainerType, typename T>
class PredicateCounterMock : public Metal::Mock<PredicateCounter<ContainerType, T>>
{
public:
   using PredicateFunctionPointerType = bool(*)(const T&);
   METALMOCK_NONVOID2_CONST(size_t, CountWhere, const ContainerType&, PredicateFunctionPointerType)
};
