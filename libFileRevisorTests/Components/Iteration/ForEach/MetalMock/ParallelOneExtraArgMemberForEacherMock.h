#pragma once
#include "libFileRevisor/Components/Iteration/ForEach/ParallelOneExtraArgMemberForEacher.h"

template<typename T, typename ClassType, typename MemberFunctionType, typename ExtraArgType>
class ParallelOneExtraArgMemberForEacherMock : public Zen::Mock<ParallelOneExtraArgMemberForEacher<T, ClassType, MemberFunctionType, ExtraArgType>>
{
public:
   METALMOCK_VOID4_CONST(ParallelOneExtraArgMemberForEach, const vector<T>&, const ClassType*, MemberFunctionType, ExtraArgType)
};
