#pragma once
#include "libFileRevisor/Components/Strings/ConstCharPointerGetter.h"

class ConstCharPointerGetterMock : public Zen::Mock<ConstCharPointerGetter>
{
public:
   ZENMOCK_NONVOID1_CONST(const char*, GetStringConstCharPointer, const std::string&)
};
