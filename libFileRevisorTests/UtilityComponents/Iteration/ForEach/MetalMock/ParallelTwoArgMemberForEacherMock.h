#pragma once
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/ParallelTwoArgMemberForEacher.h"

template<typename ClassType, typename T, typename Arg2Type>
class ParallelTwoArgMemberForEacherMock : public Metal::Mock<ParallelTwoArgMemberForEacher<ClassType, T, Arg2Type>>
{
public:
   using ConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type) const;
   METALMOCK_VOID4_CONST(ParallelCallConstMemberFunctionWithEachElement, const vector<T>&, const ClassType*, ConstMemberFunctionType, Arg2Type)
};
