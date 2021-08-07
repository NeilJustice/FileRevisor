#pragma once
#include "libFileRevisor/UtilityComponents/DataStructures/CharArray64Helper.h"

class CharArray64HelperMock : public Metal::Mock<CharArray64Helper>
{
public:
   using CharArray64Type = array<char, 64>;
   METALMOCK_NONVOID2_CONST(bool, ArrayContains0, const CharArray64Type&, size_t)
};
