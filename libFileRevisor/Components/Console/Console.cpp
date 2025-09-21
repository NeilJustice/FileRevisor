#include "pch.h"
#include "libFileRevisor/Components/Console/Console.h"
#include "libFileRevisor/Components/Console/ConsoleColorer.h"

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

void Console::ProgramNameThreadIdWriteLine(string_view message) const
{
   const string threadIdMessage = MakeThreadIdMessage(message);
   scoped_lock<mutex> coutLock(_coutMutex);
   cout << threadIdMessage;
}

void Console::ProgramNameThreadIdWriteLineColor(string_view message, Color color) const
{
   const string threadIdMessage = MakeThreadIdMessage(message);
   scoped_lock<mutex> coutLock(_coutMutex);
   const bool didSetTextColor = _consoleColorer->SetTextColor(color);
   cout << threadIdMessage;
   _consoleColorer->UnsetTextColor(didSetTextColor);
}

// Private Functions

string Console::MakeThreadIdMessage(string_view message)
{
   const thread::id threadId = this_thread::get_id();
   ostringstream threadIdMessageBuilder;
   threadIdMessageBuilder << "[FileRevisor T" << left << setw(5) << threadId << "] " << message << '\n';
   string threadIdMessage = threadIdMessageBuilder.str();
   return threadIdMessage;
}
