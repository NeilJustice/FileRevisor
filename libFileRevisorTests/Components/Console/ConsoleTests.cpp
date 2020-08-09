#include "pch.h"
#include "libFileRevisor/Components/Console/Console.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidZeroArgMemberFunctionCaller.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/VoidZeroArgMemberFunctionCallerMock.h"

TESTS(ConsoleTests)
AFACT(WriteInt_CodeCoverage)
AFACT(Write_CodeCoverage)
AFACT(WriteNewLine_CodeCoverage)
AFACT(WriteLine_CodeCoverage)
AFACT(LockingWriteLine_CodeCoverage)
EVIDENCE

Console _console;

TEST(WriteInt_CodeCoverage)
{
   _console.WriteInt(0);
}

TEST(Write_CodeCoverage)
{
   _console.Write("");
   _console.Write(ZenUnit::Random<string>());
   const string_view stringView;
   _console.Write(stringView);
}

TEST(WriteNewLine_CodeCoverage)
{
   _console.WriteNewLine();
}

TEST(WriteLine_CodeCoverage)
{
   _console.WriteLine("");
   _console.WriteLine(ZenUnit::Random<string>());
   const string_view stringView;
   _console.WriteLine(stringView);
}

TEST(LockingWriteLine_CodeCoverage)
{
   _console.LockingWriteLine("");
   _console.LockingWriteLine(ZenUnit::Random<string>());
   const string_view stringView;
   _console.LockingWriteLine(stringView);
}

RUN_TESTS(ConsoleTests)
