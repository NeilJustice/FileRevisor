#pragma once
class ConsoleTests;

template<typename ClassType>
class VoidZeroArgMemberFunctionCaller;

class Console
{
   friend class ::ConsoleTests;
public:
   virtual ~Console() = default;
   virtual void WriteInt(int Value) const;
   virtual void Write(string_view message) const;
   virtual void WriteNewLine() const;
   virtual void WriteLine(string_view message) const;
   virtual void LockingWriteLine(string_view message) const;
};
