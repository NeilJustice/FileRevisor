#pragma once
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/ThreeArgMemberForEacher.h"

template<typename ClassType, typename T, typename Arg2Type, typename Arg3Type>
class ThreeArgMemberForEacherMock : public Metal::Mock<ThreeArgMemberForEacher<ClassType, T, Arg2Type, Arg3Type>>
{
public:
   using ConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type, Arg3Type) const;
   METALMOCK_VOID5_CONST(CallConstMemberFunctionWithEachElement, const vector<T>&, const ClassType*, ConstMemberFunctionType, Arg2Type, Arg3Type)
};
