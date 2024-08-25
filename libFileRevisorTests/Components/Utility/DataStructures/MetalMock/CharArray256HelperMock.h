#pragma once
#include "libFileRevisor/Components/Utility/DataStructures/CharArray256Helper.h"

class CharArray256HelperMock : public Metal::Mock<CharArray256Helper>
{
public:
   using CharArray256Type = array<char, 256>;
   METALMOCK_NONVOID2_CONST(bool, ArrayContains0, const CharArray256Type&, size_t)
};
