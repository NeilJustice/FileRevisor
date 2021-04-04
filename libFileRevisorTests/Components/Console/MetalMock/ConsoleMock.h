#pragma once
#include "libFileRevisor/Components/Console/Console.h"

class ConsoleMock : public Metal::Mock<Console>
{
public:
   METALMOCK_VOID1_CONST(WriteLine, string_view)
};
