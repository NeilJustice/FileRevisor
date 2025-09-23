#include "pch.h"
#include "libFileRevisor/Components/Threading/ThreadIdGetter.h"

thread::id ThreadIdGetter::GetThreadId() const
{
   thread::id threadId = this_thread::get_id();
   return threadId;
}
