#include "pch.h"
#include "libFileRevisor/UtilityComponents/Threading/ThreadIdGetter.h"

ThreadIdGetter::~ThreadIdGetter()
{
}

thread::id ThreadIdGetter::GetThreadId() const
{
   const thread::id threadId = this_thread::get_id();
   return threadId;
}
