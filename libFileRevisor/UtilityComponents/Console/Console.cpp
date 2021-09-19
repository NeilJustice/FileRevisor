#include "pch.h"
#include "libFileRevisor/UtilityComponents/Console/Console.h"
#include "libFileRevisor/UtilityComponents/Console/ConsoleColorer.h"

Console::Console()
   // Mutable Components
   : _consoleColorer(make_unique<ConsoleColorer>())
{
}

Console::~Console()
{
}

void Console::WriteLine(string_view message) const
{
   scoped_lock<mutex> coutLock(_coutMutex);
   cout << message << '\n';
}

void Console::ThreadIdWriteLine(string_view message) const
{
   const thread::id threadId = this_thread::get_id();
   const string fullMessage = String::ConcatValues("[FileRevisor Thread ", threadId, "] ", message, "\n");
   scoped_lock<mutex> coutLock(_coutMutex);
   cout << fullMessage;
}

void Console::ThreadIdWriteLineColor(string_view message, Color color) const
{
   const thread::id threadId = this_thread::get_id();
   const string fullMessage = String::ConcatValues("[FileRevisor Thread ", threadId, "] ", message, "\n");
   scoped_lock<mutex> coutLock(_coutMutex);
   const bool didSetTextColor = _consoleColorer->SetTextColor(color);
   cout << fullMessage;
   _consoleColorer->UnsetTextColor(didSetTextColor);
}
