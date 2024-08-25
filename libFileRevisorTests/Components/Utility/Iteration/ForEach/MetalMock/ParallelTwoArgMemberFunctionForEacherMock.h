#pragma once
#include "libFileRevisor/Components/Utility/Iteration/ForEach/ParallelTwoArgMemberFunctionForEacher.h"

template<typename ClassType, typename T, typename Arg2Type>
class ParallelTwoArgMemberFunctionForEacherMock : public Metal::Mock<ParallelTwoArgMemberFunctionForEacher<ClassType, T, Arg2Type>>
{
public:
   using ConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type) const;
   METALMOCK_VOID4_CONST(ParallelCallConstMemberFunctionWithEachElement, const vector<T>&, const ClassType*, ConstMemberFunctionType, Arg2Type)
};
