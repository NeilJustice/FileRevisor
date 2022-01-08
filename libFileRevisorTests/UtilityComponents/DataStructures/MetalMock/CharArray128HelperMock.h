#pragma once
#include "libFileRevisor/UtilityComponents/DataStructures/CharArray128Helper.h"

class CharArray128HelperMock : public Metal::Mock<CharArray128Helper>
{
public:
   using CharArray128Type = array<char, 128>;
   METALMOCK_NONVOID2_CONST(bool, ArrayContains0, const CharArray128Type&, size_t)
};
