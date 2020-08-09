#pragma once
#include "libFileRevisor/Components/Strings/Regexer.h"

class RegexerMock : public Zen::Mock<Regexer>
{
public:
   ZENMOCK_NONVOID3_CONST(string, Replace, string_view, string_view, string_view)
};
