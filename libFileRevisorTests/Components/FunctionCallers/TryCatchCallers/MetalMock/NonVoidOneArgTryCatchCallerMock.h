#pragma once
#include "libFileRevisor/Components/FunctionCallers/TryCatchCallers/NonVoidOneArgTryCatchCaller.h"

template<typename ReturnType, typename ClassType, typename ArgumentType>
class NonVoidOneArgTryCatchCallerMock : public Metal::Mock<NonVoidOneArgTryCatchCaller<ReturnType, ClassType, ArgumentType>>
{
public:
   using ConstMemberFunctionType = ReturnType (ClassType::*)(ArgumentType) const;
   using ExceptionHandlerFunctionType = ReturnType (ClassType::*)(const exception&) const;
   METALMOCK_NONVOID4_CONST(ReturnType, TryCatchCallConstMemberFunction,
      const ClassType*, ConstMemberFunctionType, ArgumentType, ExceptionHandlerFunctionType)
};
