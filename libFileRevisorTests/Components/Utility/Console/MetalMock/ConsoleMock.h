#pragma once
#include "libFileRevisor/Components/Utility/Console/Console.h"

class ConsoleMock : public Metal::Mock<Console>
{
public:
   METALMOCK_VOID1_CONST(WriteLine, string_view)
   METALMOCK_VOID1_CONST(ProgramNameThreadIdWriteLine, string_view)
   METALMOCK_VOID2_CONST(ProgramNameThreadIdWriteLineColor, string_view, Color)
};
