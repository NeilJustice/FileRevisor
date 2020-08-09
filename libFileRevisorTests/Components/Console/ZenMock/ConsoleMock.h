#pragma once
#include "libFileRevisor/Components/Console/Console.h"

class ConsoleMock : public Zen::Mock<Console>
{
public:
   ZENMOCK_VOID1_CONST(WriteInt, int)
   ZENMOCK_VOID1_CONST(Write, string_view)
   ZENMOCK_VOID0_CONST(WriteNewLine)
   ZENMOCK_VOID1_CONST(WriteLine, string_view)
   ZENMOCK_VOID1_CONST(LockingWriteLine, string_view)
};
