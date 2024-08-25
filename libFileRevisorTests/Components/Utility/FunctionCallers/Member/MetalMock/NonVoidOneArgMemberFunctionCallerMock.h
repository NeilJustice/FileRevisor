#pragma once
#include "libFileRevisor/Components/Utility/FunctionCallers/Member/NonVoidOneArgMemberFunctionCaller.h"

template<typename ReturnType, typename ClassType, typename ArgType>
class NonVoidOneArgMemberFunctionCallerMock : public Metal::Mock<NonVoidOneArgMemberFunctionCaller<ReturnType, ClassType, ArgType>>
{
public:
   using MemberFunctionType = ReturnType(ClassType::*)(ArgType) const;
   METALMOCK_NONVOID3_CONST(ReturnType, CallConstMemberFunction, const ClassType*, MemberFunctionType, ArgType)
};
