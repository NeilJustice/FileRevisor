#pragma once
#include "libFileRevisor/Components/FunctionCallers/Member/VoidZeroArgMemberFunctionCaller.h"

template<typename ClassType>
class VoidZeroArgMemberFunctionCallerMock : public Metal::Mock<VoidZeroArgMemberFunctionCaller<ClassType>>
{
public:
   using ConstMemberFunctionType = void (ClassType::*)() const;
   METALMOCK_VOID2_CONST(CallConstMemberFunction, const ClassType*, ConstMemberFunctionType)

   using NonConstMemberFunctionType = void (ClassType::*)();
   METALMOCK_VOID2_CONST(CallNonConstMemberFunction, ClassType*, NonConstMemberFunctionType)
};
