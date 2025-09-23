#pragma once
#include "libFileRevisor/Components/FunctionCallers/TryCatchCallers/NonVoidOneArgTryCatchCaller.h"

template<typename ReturnType, typename ClassType, typename ArgType>
class NonVoidOneArgTryCatchCallerMock : public Metal::Mock<NonVoidOneArgTryCatchCaller<ReturnType, ClassType, ArgType>>
{
public:
   using ConstMemberFunctionType = ReturnType (ClassType::*)(ArgType) const;
   using ExceptionHandlerFunctionType = ReturnType (ClassType::*)(const exception&) const;

   METALMOCK_NONVOID4_CONST(ReturnType, TryCatchCallConstMemberFunction,
      const ClassType*, ConstMemberFunctionType, ArgType, ExceptionHandlerFunctionType)
};
