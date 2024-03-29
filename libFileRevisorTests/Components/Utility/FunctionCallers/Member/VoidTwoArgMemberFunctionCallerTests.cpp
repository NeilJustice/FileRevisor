#include "pch.h"
#include "libFileRevisor/Components/Utility/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"

template<typename Arg1Type, typename Arg2Type>
TEMPLATE_TESTS(VoidTwoArgMemberFunctionCallerTests, Arg1Type, Arg2Type)
AFACT(CallConstMemberFunction_CallsConstMemberFunctionOnce)
AFACT(CallNonConstMemberFunction_CallsNonConstMemberFunctionOnce)
EVIDENCE

class Class
{
public:
   mutable vector<pair<Arg1Type, Arg2Type>> calls;

   void ConstMemberFunction(Arg1Type arg1, Arg2Type arg2) const
   {
      calls.emplace_back(arg1, arg2);
   }

   void NonConstMemberFunction(Arg1Type arg1, Arg2Type arg2)
   {
      calls.emplace_back(arg1, arg2);
   }
};

TEST(CallConstMemberFunction_CallsConstMemberFunctionOnce)
{
   Class classInstance{};
   VoidTwoArgMemberFunctionCaller<Class, Arg1Type, Arg2Type> voidTwoArgMemberFunctionCaller;
   IS_EMPTY(classInstance.calls);
   //
   voidTwoArgMemberFunctionCaller.CallConstMemberFunction(&classInstance, &Class::ConstMemberFunction, Arg1Type{ 1 }, Arg2Type{ 2 });
   //
   vector<pair<Arg1Type, Arg2Type>> expectedCalls = { { Arg1Type{ 1 }, Arg2Type{ 2 } } };
   VECTORS_ARE_EQUAL(expectedCalls, classInstance.calls);


   voidTwoArgMemberFunctionCaller.CallConstMemberFunction(&classInstance, &Class::ConstMemberFunction, Arg1Type{ 3 }, Arg2Type{ 4 });
   //
   expectedCalls.emplace_back(Arg1Type{ 3 }, Arg2Type{ 4 });
   VECTORS_ARE_EQUAL(expectedCalls, classInstance.calls);
}

TEST(CallNonConstMemberFunction_CallsNonConstMemberFunctionOnce)
{
   Class classInstance{};
   VoidTwoArgMemberFunctionCaller<Class, Arg1Type, Arg2Type> voidTwoArgMemberFunctionCaller;
   IS_EMPTY(classInstance.calls);
   //
   voidTwoArgMemberFunctionCaller.CallNonConstMemberFunction(&classInstance, &Class::NonConstMemberFunction, Arg1Type{ 1 }, Arg2Type{ 2 });
   //
   vector<pair<Arg1Type, Arg2Type>> expectedCalls = { { Arg1Type{ 1 }, Arg2Type{ 2 } } };
   VECTORS_ARE_EQUAL(expectedCalls, classInstance.calls);


   voidTwoArgMemberFunctionCaller.CallNonConstMemberFunction(&classInstance, &Class::NonConstMemberFunction, Arg1Type{ 3 }, Arg2Type{ 4 });
   //
   expectedCalls.emplace_back(Arg1Type{ 3 }, Arg2Type{ 4 });
   VECTORS_ARE_EQUAL(expectedCalls, classInstance.calls);
}

RUN_TEMPLATE_TESTS(VoidTwoArgMemberFunctionCallerTests, int, char)
THEN_RUN_TEMPLATE_TESTS(VoidTwoArgMemberFunctionCallerTests, int, unsigned long long)
