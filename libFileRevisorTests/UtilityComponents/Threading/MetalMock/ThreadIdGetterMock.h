#pragma once
#include "libFileRevisor/UtilityComponents/Threading/ThreadIdGetter.h"

namespace ZenUnit
{
   template<>
   thread::id Random();
}

class ThreadIdGetterMock : public Metal::Mock<ThreadIdGetter>
{
public:
   METALMOCK_NONVOID0_CONST(thread::id, GetThreadId)
};
