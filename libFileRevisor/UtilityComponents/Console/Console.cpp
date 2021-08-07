#include "pch.h"
#include "libFileRevisor/UtilityComponents/Console/Console.h"

void Console::WriteLine(string_view message) const
{
   scoped_lock<mutex> coutLock(_coutMutex);
   cout << message << '\n';
}
