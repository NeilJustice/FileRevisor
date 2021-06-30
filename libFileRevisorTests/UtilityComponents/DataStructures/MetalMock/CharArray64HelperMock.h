#pragma once
#include "libFileRevisor/UtilityComponents/DataStructures/CharArray64Helper.h"

class CharArray64HelperMock : public Metal::Mock<CharArray64Helper>
{
public:
   using CharArray64Type = array<char, 64>;
   METALMOCK_NONVOID3_CONST(bool, ArrayContains, const CharArray64Type&, char, size_t)
};
