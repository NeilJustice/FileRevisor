#pragma once
#include "libFileRevisor/Components/Utility/Console/Console.h"

class ConsoleMock : public Metal::Mock<Console>
{
public:
   METALMOCK_VOID1_CONST(WriteLine, string_view)
   METALMOCK_VOID1_CONST(ThreadIdWriteLine, string_view)
   METALMOCK_VOID2_CONST(ThreadIdWriteLineColor, string_view, Color)
};
