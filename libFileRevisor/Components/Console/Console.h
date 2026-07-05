#pragma once
#include <execution>
class ConsoleColorer;

class Console
{
   friend class ConsoleTests;
private:
   // Mutable Components
   unique_ptr<ConsoleColorer> p_consoleColorer;
   // Mutable Fields
   mutable mutex _coutMutex;
public:
   Console();
   virtual ~Console();
   virtual void WriteLine(string_view message) const;
   virtual void WriteProgramNameThreadIdLine(string_view message) const;
   virtual void WriteProgramNameThreadIdLineColor(string_view message, Color color) const;
private:
   static string MakeThreadIdMessage(string_view message);
};
