#include "pch.h"
#include "libFileRevisor/Components/Iteration/ForEach/ParallelOneExtraArgMemberForEacher.h"

class Class {};
using MemberFunctionType = void(Class::*)(int, double) const;
template class ParallelOneExtraArgMemberForEacher<int, Class, MemberFunctionType, double>;
