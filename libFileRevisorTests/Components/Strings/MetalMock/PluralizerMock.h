#pragma once
#include "libFileRevisor/Components/Strings/Pluralizer.h"

class PluralizerMock : public Zen::Mock<Pluralizer>
{
public:
   METALMOCK_NONVOID3_CONST(string, PotentiallyPluralizeWord, size_t, string_view, string_view)
};
