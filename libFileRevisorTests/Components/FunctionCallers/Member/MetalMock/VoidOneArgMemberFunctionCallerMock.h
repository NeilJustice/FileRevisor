#pragma once
#include "libFileRevisor/Components/FunctionCallers/Member/VoidOneArgMemberFunctionCaller.h"

template<typename ClassType, typename Arg1Type>
class VoidOneArgMemberFunctionCallerMock : public Metal::Mock<VoidOneArgMemberFunctionCaller<ClassType, Arg1Type>>
{
public:
   using ConstMemberFunctionType = void (ClassType::*)(Arg1Type) const;
   METALMOCK_VOID3_CONST(CallConstMemberFunction, const ClassType*, ConstMemberFunctionType, Arg1Type)

   using NonConstMemberFunctionType = void (ClassType::*)(Arg1Type);
   METALMOCK_VOID3_CONST(CallNonConstMemberFunction, ClassType*, NonConstMemberFunctionType, Arg1Type)
};
