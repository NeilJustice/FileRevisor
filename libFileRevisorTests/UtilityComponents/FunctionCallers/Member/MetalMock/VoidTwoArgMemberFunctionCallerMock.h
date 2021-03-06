#pragma once
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCallerMock : public Metal::Mock<VoidTwoArgMemberFunctionCaller<ClassType, Arg1Type, Arg2Type>>
{
public:
   using ConstMemberFunctionType = void (ClassType::*)(Arg1Type, Arg2Type) const;
   METALMOCK_VOID4_CONST(ConstCall, const ClassType*, ConstMemberFunctionType, Arg1Type, Arg2Type)

   using NonConstMemberFunctionType = void (ClassType::*)(Arg1Type, Arg2Type);
   METALMOCK_VOID4_CONST(NonConstCall, ClassType*, NonConstMemberFunctionType, Arg1Type, Arg2Type)
};
