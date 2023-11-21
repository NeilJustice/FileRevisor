#include "pch.h"
#include "libFileRevisor/Components/Utility/Console/Console.h"
#include "libFileRevisorTests/Components/Utility/Console/MetalMock/ConsoleColorerMock.h"

TESTS(ConsoleTests)
AFACT(DefaultConstructor_NewsConsoleColorer)
AFACT(WriteLine_CodeCoverage)
AFACT(ProgramNameThreadIdWriteLine_CodeCoverage)
AFACT(ProgramNameThreadIdWriteLineColor_SetsConsoleTextColor_WritesMessageThenNewline_UnsetsColor)
EVIDENCE

Console _console;
// Mutable Components
ConsoleColorerMock* _consoleColorerMock = nullptr;

STARTUP
{
   // Mutable Components
   _console._consoleColorer.reset(_consoleColorerMock = new ConsoleColorerMock);
}

TEST(DefaultConstructor_NewsConsoleColorer)
{
   Console console;
   // Function Pointers
   DELETE_TO_ASSERT_NEWED(console._consoleColorer);
}

TEST(WriteLine_CodeCoverage)
{
   _console.WriteLine(ZenUnit::Random<string>());
}

TEST(ProgramNameThreadIdWriteLine_CodeCoverage)
{
   _console.ProgramNameThreadIdWriteLine(ZenUnit::Random<string>());
}

TEST(ProgramNameThreadIdWriteLineColor_SetsConsoleTextColor_WritesMessageThenNewline_UnsetsColor)
{
   const bool didSetTextColor = _consoleColorerMock->SetTextColorMock.ReturnRandom();
   _consoleColorerMock->UnsetTextColorMock.Expect();
   const string message = ZenUnit::Random<string>();
   const Color color = ZenUnit::RandomEnum<Color>();
   //
   _console.ProgramNameThreadIdWriteLineColor(message, color);
   //
   METALMOCK(_consoleColorerMock->SetTextColorMock.CalledOnceWith(color));
   METALMOCK(_consoleColorerMock->UnsetTextColorMock.CalledOnceWith(didSetTextColor));
}

RUN_TESTS(ConsoleTests)
