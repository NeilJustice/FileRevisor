#pragma once
#include "libFileRevisor/Components/FunctionCallers/TryCatchCallers/VoidOneArgTryCatchCaller.h"

template<typename ClassType, typename Arg1Type>
class VoidOneArgTryCatchCallerMock : public Metal::Mock<VoidOneArgTryCatchCaller<ClassType, Arg1Type>>
{
public:
   using ConstMemberFunctionType = void (ClassType::*)(Arg1Type) const;
   using ConstMemberExceptionFunctionType = void (ClassType::*)(string_view) const;
   using NonConstMemberFunctionType = void (ClassType::*)(Arg1Type);

   METALMOCK_VOID4_CONST(TryCatchCallConstMemberFunction,
      const ClassType*, ConstMemberFunctionType,
      Arg1Type,
      ConstMemberExceptionFunctionType)

   METALMOCK_VOID4_CONST(TryCatchCallNonConstMemberFunction,
      ClassType*, NonConstMemberFunctionType,
      Arg1Type,
      ConstMemberExceptionFunctionType)
};
