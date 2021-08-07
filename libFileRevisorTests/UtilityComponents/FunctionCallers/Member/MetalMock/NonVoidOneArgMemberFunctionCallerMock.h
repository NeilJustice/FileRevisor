#pragma once
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/NonVoidOneArgMemberFunctionCaller.h"

template<typename ReturnType, typename ClassType, typename ArgType>
class NonVoidOneArgMemberFunctionCallerMock : public Metal::Mock<NonVoidOneArgMemberFunctionCaller<ReturnType, ClassType, ArgType>>
{
public:
   using MemberFunctionType = ReturnType(ClassType::*)(ArgType) const;
   METALMOCK_NONVOID3_CONST(ReturnType, ConstCall, const ClassType*, MemberFunctionType, ArgType)
};
