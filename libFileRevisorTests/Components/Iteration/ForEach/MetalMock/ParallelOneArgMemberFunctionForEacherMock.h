#pragma once
#include "libFileRevisor/Components/Iteration/ForEach/ParallelOneArgMemberFunctionForEacher.h"

template<typename ClassType, typename T>
class ParallelOneArgMemberFunctionForEacherMock : public Metal::Mock<ParallelOneArgMemberFunctionForEacher<ClassType, T>>
{
public:
   using ConstMemberFunctionType = void(ClassType::*)(const T&) const;

   METALMOCK_VOID3_CONST(ParallelCallConstMemberFunctionWithEachElement, const vector<T>&, const ClassType*, ConstMemberFunctionType)
};
