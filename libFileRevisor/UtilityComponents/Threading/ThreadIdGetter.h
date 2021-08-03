#pragma once

class ThreadIdGetter
{
public:
   virtual ~ThreadIdGetter();
   virtual thread::id GetThreadId() const;
};
