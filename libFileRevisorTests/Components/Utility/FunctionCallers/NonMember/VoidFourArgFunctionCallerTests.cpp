#include "pch.h"
#include "libFileRevisor/Components/Utility/FunctionCallers/NonMember/VoidFourArgFunctionCaller.h"

template<typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
TEMPLATE_TESTS(VoidFourArgFunctionCallerTests, Arg1Type, Arg2Type, Arg3Type, Arg4Type)
AFACT(Call_CallsFunctionOnce)
EVIDENCE

static vector<tuple<Arg1Type, Arg2Type, Arg3Type, Arg4Type>> _functionCalls;

static void VoidFourArgFunction(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3, Arg4Type arg4)
{
   _functionCalls.emplace_back(arg1, arg2, arg3, arg4);
}

CLEANUP
{
   _functionCalls.clear();
}

TEST(Call_CallsFunctionOnce)
{
   VoidFourArgFunctionCaller<Arg1Type, Arg2Type, Arg3Type, Arg4Type> voidFourArgFunctionCaller;

   voidFourArgFunctionCaller.Call(VoidFourArgFunction, Arg1Type{ 1 }, Arg2Type{ 2 }, Arg3Type{ 3 }, Arg4Type{ 4 });
   vector<tuple<Arg1Type, Arg2Type, Arg3Type, Arg4Type>> expectedCalls = { { Arg1Type{ 1 }, Arg2Type{ 2 }, Arg3Type{ 3 }, Arg4Type{ 4 } } };
   VECTORS_ARE_EQUAL(expectedCalls, _functionCalls);

   voidFourArgFunctionCaller.Call(VoidFourArgFunction, Arg1Type{ 3 }, Arg2Type{ 4 }, Arg3Type{ 5 }, Arg4Type{ 6 });
   expectedCalls.emplace_back(Arg1Type{ 3 }, Arg2Type{ 4 }, Arg3Type{ 5 }, Arg4Type{ 6 });
   VECTORS_ARE_EQUAL(expectedCalls, _functionCalls);
}

RUN_TEMPLATE_TESTS(VoidFourArgFunctionCallerTests, int, char, unsigned, int)
THEN_RUN_TEMPLATE_TESTS(VoidFourArgFunctionCallerTests, int, unsigned long long, char, int)

template<typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
vector<tuple<Arg1Type, Arg2Type, Arg3Type, Arg4Type>> VoidFourArgFunctionCallerTests<Arg1Type, Arg2Type, Arg3Type, Arg4Type>::_functionCalls;
