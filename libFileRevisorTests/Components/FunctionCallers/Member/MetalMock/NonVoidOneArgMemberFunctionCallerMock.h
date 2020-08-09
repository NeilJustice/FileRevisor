#pragma once
#include "libFileRevisor/Components/FunctionCallers/Member/NonVoidOneArgMemberFunctionCaller.h"

template<typename ReturnType, typename ClassType, typename ArgType>
class NonVoidOneArgMemberFunctionCallerMock : public Zen::Mock<NonVoidOneArgMemberFunctionCaller<ReturnType, ClassType, ArgType>>
{
public:
   using MemberFunctionType = ReturnType(ClassType::*)(ArgType) const;
   METALMOCK_NONVOID3_CONST(ReturnType, ConstCall, const ClassType*, MemberFunctionType, ArgType)
};
