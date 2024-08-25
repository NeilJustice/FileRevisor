#pragma once
#include "libFileRevisor/Components/Utility/Strings/ConstCharPointerGetter.h"

class ConstCharPointerGetterMock : public Metal::Mock<ConstCharPointerGetter>
{
public:
   METALMOCK_NONVOID1_CONST(const char*, GetStringConstCharPointer, const std::string&)
};
