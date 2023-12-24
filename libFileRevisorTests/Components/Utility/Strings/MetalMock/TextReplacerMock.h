#pragma once
#include "libFileRevisor/Components/Utility/Strings/TextReplacer.h"

class TextReplacerMock : public Metal::Mock<TextReplacer>
{
public:
   METALMOCK_NONVOID3_CONST(string, ReplaceText, string_view, string_view, const string&)
};
