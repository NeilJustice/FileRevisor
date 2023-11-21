#pragma once
class ConsoleColorer;

class Console
{
   friend class ConsoleTests;
private:
   // Mutable Components
   unique_ptr<ConsoleColorer> _consoleColorer;
   // Mutable Fields
   mutable mutex _coutMutex;
public:
   Console();
   virtual ~Console();
   virtual void WriteLine(string_view message) const;
   virtual void ProgramNameThreadIdWriteLine(string_view message) const;
   virtual void ProgramNameThreadIdWriteLineColor(string_view message, Color color) const;
private:
   static string MakeThreadIdMessage(string_view message);
};
