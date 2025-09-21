#pragma once
#include "libFileRevisor/Components/Iteration/ForEach/FourArgMemberFunctionForEacher.h"

namespace Utils
{
   template<typename ClassType, typename T, typename Arg2Type, typename Arg3Type, typename Arg4Type>
   class FourArgMemberFunctionForEacherMock : public Metal::Mock<FourArgMemberFunctionForEacher<ClassType, T, Arg2Type, Arg3Type, Arg4Type>>
   {
   public:
      using ConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type, Arg3Type, Arg4Type) const;
      METALMOCK_VOID6_CONST(CallConstMemberFunctionWithEachElement,
         const vector<T>&, const ClassType*, ConstMemberFunctionType, Arg2Type, Arg3Type, Arg4Type)

      using NonConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type, Arg3Type, Arg4Type);
      METALMOCK_VOID6_CONST(CallNonConstMemberFunctionWithEachElement,
         const vector<T>&, ClassType*, NonConstMemberFunctionType, Arg2Type, Arg3Type, Arg4Type)
   };
}
