#pragma once
#include "libFileRevisor/Components/FunctionCallers/NonMember/VoidFourArgFunctionCaller.h"

template<typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
class VoidFourArgMemberFunctionCallerMock : public Metal::Mock<VoidFourArgFunctionCaller<Arg1Type, Arg2Type, Arg3Type, Arg4Type>>
{
public:
   using VoidFourArgFunctionType = void (*)(Arg1Type, Arg2Type, Arg3Type, Arg4Type);
   METALMOCK_VOID5_CONST(Call, VoidFourArgFunctionType, Arg1Type, Arg2Type, Arg3Type, Arg4Type)
};
