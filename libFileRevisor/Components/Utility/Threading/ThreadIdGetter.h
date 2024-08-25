#pragma once

class ThreadIdGetter
{
public:
   virtual ~ThreadIdGetter() = default;
   virtual thread::id GetThreadId() const;
};
