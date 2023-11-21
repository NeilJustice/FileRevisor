#pragma once
#include "libFileRevisor/Components/Utility/Strings/RegexReplacer.h"

class RegexReplacerMock : public Metal::Mock<RegexReplacer>
{
public:
   METALMOCK_NONVOID3_CONST(string, RegexReplace, string_view, string_view, string_view)
};
