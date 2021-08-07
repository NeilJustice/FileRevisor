#include "pch.h"
#include "libFileRevisor/UtilityComponents/Console/Console.h"

TESTS(ConsoleTests)
AFACT(WriteLine_CodeCoverage)
EVIDENCE

Console _console;

TEST(WriteLine_CodeCoverage)
{
   _console.WriteLine(ZenUnit::Random<string>());
}

RUN_TESTS(ConsoleTests)
