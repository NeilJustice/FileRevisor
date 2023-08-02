#include "pch.h"
#include "libFileRevisor/Components/Utility/Threading/ThreadIdGetter.h"

thread::id ThreadIdGetter::GetThreadId() const
{
   const thread::id threadId = this_thread::get_id();
   return threadId;
}
