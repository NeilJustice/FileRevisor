#include "pch.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidOneArgMemberFunctionCaller.h"

template<typename Arg1Type>
TEMPLATE_TESTS(VoidOneArgMemberFunctionCallerTests, Arg1Type)
AFACT(CallConstMemberFunction_CallsConstMemberFunctionOnce)
AFACT(CallNonConstMemberFunction_CallsNonConstMemberFunctionOnce)
EVIDENCE

class Class
{
public:
   mutable vector<Arg1Type> calls;

   void ConstMemberFunction(Arg1Type arg1) const
   {
      calls.push_back(arg1);
   }

   void NonConstMemberFunction(Arg1Type arg1)
   {
      calls.push_back(arg1);
   }
};

TEST(CallConstMemberFunction_CallsConstMemberFunctionOnce)
{
   Class classInstance{};
   VoidOneArgMemberFunctionCaller<Class, Arg1Type> voidOneArgMemberFunctionCaller;
   IS_EMPTY(classInstance.calls);
   //
   voidOneArgMemberFunctionCaller.CallConstMemberFunction(&classInstance, &Class::ConstMemberFunction, Arg1Type{ 1 });
   //
   vector<Arg1Type> expectedCalls = { { Arg1Type{ 1 } } };
   VECTORS_ARE_EQUAL(expectedCalls, classInstance.calls);


   voidOneArgMemberFunctionCaller.CallConstMemberFunction(&classInstance, &Class::ConstMemberFunction, Arg1Type{ 3 });
   //
   expectedCalls.push_back(Arg1Type{ 3 });
   VECTORS_ARE_EQUAL(expectedCalls, classInstance.calls);
}

TEST(CallNonConstMemberFunction_CallsNonConstMemberFunctionOnce)
{
   Class classInstance{};
   VoidOneArgMemberFunctionCaller<Class, Arg1Type> voidOneArgMemberFunctionCallerTests;
   IS_EMPTY(classInstance.calls);
   //
   voidOneArgMemberFunctionCallerTests.CallNonConstMemberFunction(&classInstance, &Class::NonConstMemberFunction, Arg1Type{ 1 });
   //
   vector<Arg1Type> expectedCalls = { { Arg1Type{ 1 } } };
   VECTORS_ARE_EQUAL(expectedCalls, classInstance.calls);


   voidOneArgMemberFunctionCallerTests.CallNonConstMemberFunction(&classInstance, &Class::NonConstMemberFunction, Arg1Type{ 3 });
   //
   expectedCalls.push_back(Arg1Type{ 3 });
   VECTORS_ARE_EQUAL(expectedCalls, classInstance.calls);
}

RUN_TEMPLATE_TESTS(VoidOneArgMemberFunctionCallerTests, int)
THEN_RUN_TEMPLATE_TESTS(VoidOneArgMemberFunctionCallerTests, unsigned long long)
