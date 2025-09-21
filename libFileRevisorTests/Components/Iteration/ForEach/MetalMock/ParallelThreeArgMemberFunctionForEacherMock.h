#pragma once
#include "libFileRevisor/Components/Iteration/ForEach/ParallelThreeArgMemberFunctionForEacher.h"

template<typename ClassType, typename T, typename Arg2Type, typename Arg3Type>
class ParallelThreeArgMemberFunctionForEacherMock : public Metal::Mock<ParallelThreeArgMemberFunctionForEacher<ClassType, T, Arg2Type, Arg3Type>>
{
public:
   using ConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type, Arg3Type) const;
   METALMOCK_VOID5_CONST(ParallelCallConstMemberFunctionWithEachElement, const vector<T>&, const ClassType*, ConstMemberFunctionType, Arg2Type, Arg3Type)
};
