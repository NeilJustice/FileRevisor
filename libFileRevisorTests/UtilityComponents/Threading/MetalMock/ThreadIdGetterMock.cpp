#include "pch.h"
#include "libFileRevisorTests/UtilityComponents/Threading/MetalMock/ThreadIdGetterMock.h"

namespace ZenUnit
{
   template<>
   thread::id Random()
   {
      const thread::id randomThreadId = this_thread::get_id();
      return randomThreadId;
   }
}
