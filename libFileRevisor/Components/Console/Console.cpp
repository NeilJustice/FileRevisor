#include "pch.h"
#include "libFileRevisor/Components/Console/Console.h"

void Console::WriteInt(int value) const
{
   cout << value;
}

void Console::Write(string_view message) const
{
   cout << message;
   cout.flush(); // Explicit flush here and with endl below, otherwise Jenkins job console output does not show FileRevisor console output
}

void Console::WriteNewLine() const
{
   cout << endl;
}

void Console::WriteLine(string_view message) const
{
   cout << message << endl;
}

void Console::LockingWriteLine(string_view message) const
{
   unique_lock<mutex> coutLock(_coutMutex);
   cout << message << endl;
}
