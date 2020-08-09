#pragma once
#include "libFileRevisor/Components/Console/Console.h"

class ConsoleMock : public Metal::Mock<Console>
{
public:
   METALMOCK_VOID1_CONST(WriteInt, int)
   METALMOCK_VOID1_CONST(Write, string_view)
   METALMOCK_VOID0_CONST(WriteNewLine)
   METALMOCK_VOID1_CONST(WriteLine, string_view)
   METALMOCK_VOID1_CONST(LockingWriteLine, string_view)
};
