#pragma once
#include "libFileRevisor/Components/Utility/Iteration/ForEach/TwoArgMemberFunctionForEacher.h"

template<typename ClassType, typename T, typename Arg2Type>
class TwoArgMemberFunctionForEacherMock : public Metal::Mock<TwoArgMemberFunctionForEacher<ClassType, T, Arg2Type>>
{
public:
   using ConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type) const;
   METALMOCK_VOID4_CONST(CallConstMemberFunctionWithEachElement, const vector<T>&, const ClassType*, ConstMemberFunctionType, Arg2Type)
};
