#include "pch.h"
#include "libFileRevisor/Components/Console/Console.h"

void Console::WriteLine(string_view message) const
{
   scoped_lock<mutex> coutLock(_coutMutex);
   cout << message << '\n';
}
