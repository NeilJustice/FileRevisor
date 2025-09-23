#pragma once
#include "libFileRevisor/Components/Iteration/ForEach/OneArgMemberFunctionForEacher.h"

template<typename ClassType, typename T>
class OneArgMemberFunctionForEacherMock : public Metal::Mock<OneArgMemberFunctionForEacher<ClassType, T>>
{
public:
   using ConstMemberFunctionType = void(ClassType::*)(const T&) const;

   METALMOCK_VOID3_CONST(CallConstMemberFunctionWithEachElement, const vector<T>&, const ClassType*, ConstMemberFunctionType)
};
