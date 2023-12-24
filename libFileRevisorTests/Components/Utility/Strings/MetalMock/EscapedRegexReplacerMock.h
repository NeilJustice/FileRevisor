#pragma once
#include "libFileRevisor/Components/Utility/Strings/EscapedRegexReplacer.h"

class EscapedRegexReplacerMock : public Metal::Mock<EscapedRegexReplacer>
{
public:
   METALMOCK_NONVOID3_CONST(string, ReplaceText, string_view, string_view, string_view)
};
