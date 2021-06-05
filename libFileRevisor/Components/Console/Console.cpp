#include "pch.h"
#include "libFileRevisor/Components/Console/Console.h"
#include <syncstream>

void Console::WriteLine(string_view message) const
{
   std::osyncstream{cout} << message << '\n';
}
