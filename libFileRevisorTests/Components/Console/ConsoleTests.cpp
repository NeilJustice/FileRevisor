#include "pch.h"
#include "libFileRevisor/Components/Console/Console.h"
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleColorerMock.h"

TESTS(ConsoleTests)
AFACT(WriteLine_CodeCoverage)
AFACT(WriteProgramNameThreadIdLine_CodeCoverage)
AFACT(WriteProgramNameThreadIdLineColor_SetsConsoleTextColor_WritesMessageThenNewline_UnsetsColor)
EVIDENCE

Console p_console;
// Mutable Components
ConsoleColorerMock* p_consoleColorerMock = nullptr;

STARTUP
{
   // Mutable Components
   p_console.p_consoleColorer.reset(p_consoleColorerMock = new ConsoleColorerMock);
}

TEST(WriteLine_CodeCoverage)
{
   p_console.WriteLine(ZenUnit::Random<string>());
}

TEST(WriteProgramNameThreadIdLine_CodeCoverage)
{
   p_console.WriteProgramNameThreadIdLine(ZenUnit::Random<string>());
}

TEST(WriteProgramNameThreadIdLineColor_SetsConsoleTextColor_WritesMessageThenNewline_UnsetsColor)
{
   const bool didSetTextColor = p_consoleColorerMock->SetTextColorMock.ReturnRandom();
   p_consoleColorerMock->UnsetTextColorMock.Expect();
   const string message = ZenUnit::Random<string>();
   const Color color = ZenUnit::RandomEnum<Color>();
   //
   p_console.WriteProgramNameThreadIdLineColor(message, color);
   //
   METALMOCK(p_consoleColorerMock->SetTextColorMock.CalledOnceWith(color));
   METALMOCK(p_consoleColorerMock->UnsetTextColorMock.CalledOnceWith(didSetTextColor));
}

RUN_TESTS(ConsoleTests)
