#pragma once
#include "libFileRevisor/UtilityComponents/Strings/Regexer.h"

class RegexerMock : public Metal::Mock<Regexer>
{
public:
   METALMOCK_NONVOID3_CONST(string, Replace, string_view, string_view, string_view)
};
