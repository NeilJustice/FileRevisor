#pragma once

class Console
{
private:
   mutable mutex _coutMutex;
public:
   virtual ~Console() = default;
   virtual void WriteLine(string_view message) const;
};
