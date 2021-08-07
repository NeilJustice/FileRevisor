#pragma once
#include "libFileRevisor/UtilityComponents/Strings/ConstCharPointerGetter.h"

class ConstCharPointerGetterMock : public Metal::Mock<ConstCharPointerGetter>
{
public:
   METALMOCK_NONVOID1_CONST(const char*, GetStringConstCharPointer, const std::string&)
};
