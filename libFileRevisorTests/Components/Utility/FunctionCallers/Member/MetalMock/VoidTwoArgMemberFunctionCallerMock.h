#pragma once
#include "libFileRevisor/Components/Utility/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCallerMock : public Metal::Mock<VoidTwoArgMemberFunctionCaller<ClassType, Arg1Type, Arg2Type>>
{
public:
   using ConstMemberFunctionType = void (ClassType::*)(Arg1Type, Arg2Type) const;
   METALMOCK_VOID4_CONST(CallConstMemberFunction, const ClassType*, ConstMemberFunctionType, Arg1Type, Arg2Type)

   using NonConstMemberFunctionType = void (ClassType::*)(Arg1Type, Arg2Type);
   METALMOCK_VOID4_CONST(CallNonConstMemberFunction, ClassType*, NonConstMemberFunctionType, Arg1Type, Arg2Type)
};
