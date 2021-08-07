#pragma once
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/ParallelOneExtraArgMemberForEacher.h"

template<typename T, typename ClassType, typename MemberFunctionType, typename ExtraArgType>
class ParallelOneExtraArgMemberForEacherMock : public Metal::Mock<ParallelOneExtraArgMemberForEacher<T, ClassType, MemberFunctionType, ExtraArgType>>
{
public:
   METALMOCK_VOID4_CONST(ParallelOneExtraArgMemberForEach, const vector<T>&, const ClassType*, MemberFunctionType, ExtraArgType)
};
