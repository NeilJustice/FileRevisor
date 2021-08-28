#pragma once
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/TwoArgMemberForEacher.h"

template<typename ClassType, typename T, typename Arg2Type>
class TwoArgMemberForEacherMock : public Metal::Mock<TwoArgMemberForEacher<ClassType, T, Arg2Type>>
{
public:
   using ConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type) const;
   METALMOCK_VOID4_CONST(CallConstMemberFunctionWithEachElement, const vector<T>&, const ClassType*, ConstMemberFunctionType, Arg2Type)
};
