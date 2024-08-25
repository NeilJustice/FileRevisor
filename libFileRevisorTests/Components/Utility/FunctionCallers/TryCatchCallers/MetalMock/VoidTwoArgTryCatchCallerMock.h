#pragma once
#include "libFileRevisor/Components/Utility/FunctionCallers/TryCatchCallers/VoidTwoArgTryCatchCaller.h"

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgTryCatchCallerMock : public Metal::Mock<VoidTwoArgTryCatchCaller<ClassType, Arg1Type, Arg2Type>>
{
public:
   using ConstMemberFunctionType = void (ClassType::*)(Arg1Type, Arg2Type) const;
   using NonConstMemberFunctionType = void (ClassType::*)(Arg1Type, Arg2Type);
   using ExceptionHandlerFunctionType = void (ClassType::*)(string_view) const;
   METALMOCK_VOID5_CONST(TryCatchCallConstMemberFunction, const ClassType*, ConstMemberFunctionType, Arg1Type, Arg2Type, ExceptionHandlerFunctionType)
   METALMOCK_VOID5_CONST(TryCatchCallNonConstMemberFunction, ClassType*, NonConstMemberFunctionType, Arg1Type, Arg2Type, ExceptionHandlerFunctionType)
};
