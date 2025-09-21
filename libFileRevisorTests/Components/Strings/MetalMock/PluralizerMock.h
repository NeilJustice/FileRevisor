#pragma once
#include "libFileRevisor/Components/Strings/Pluralizer.h"

class PluralizerMock : public Metal::Mock<Pluralizer>
{
public:
   METALMOCK_NONVOID3_CONST(string, PotentiallyPluralizeWord, size_t, string_view, string_view)
};
