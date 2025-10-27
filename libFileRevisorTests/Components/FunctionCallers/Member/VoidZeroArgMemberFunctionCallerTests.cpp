#include "pch.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidZeroArgMemberFunctionCaller.h"

TESTS(VoidZeroArgMemberFunctionCallerTests)
AFACT(CallConstMemberFunction_CallsConstMemberFunction)
AFACT(CallNonConstMemberFunction_CallsNonConstMemberFunction)
EVIDENCE

struct ConstClass
{
   mutable unsigned numberOfCalls = 0;

   void ConstMemberVoidFunction() const
   {
      ++numberOfCalls;
   }
};

struct NonConstClass
{
   unsigned numberOfCalls = 0;

   void NonConstMemberVoidFunction()
   {
      ++numberOfCalls;
   }
};

TEST(CallConstMemberFunction_CallsConstMemberFunction)
{
   ConstClass c;
   VoidZeroArgMemberFunctionCaller<ConstClass> voidZeroArgMemberFunctionCaller;
   ARE_EQUAL(0, c.numberOfCalls);
   //
   voidZeroArgMemberFunctionCaller.CallConstMemberFunction(&c, &ConstClass::ConstMemberVoidFunction);
   //
   ARE_EQUAL(1, c.numberOfCalls);
   //
   voidZeroArgMemberFunctionCaller.CallConstMemberFunction(&c, &ConstClass::ConstMemberVoidFunction);
   //
   ARE_EQUAL(2, c.numberOfCalls);
}

TEST(CallNonConstMemberFunction_CallsNonConstMemberFunction)
{
   NonConstClass c;
   VoidZeroArgMemberFunctionCaller<NonConstClass> voidZeroArgMemberFunctionCaller;
   ARE_EQUAL(0, c.numberOfCalls);
   //
   voidZeroArgMemberFunctionCaller.CallNonConstMemberFunction(&c, &NonConstClass::NonConstMemberVoidFunction);
   //
   ARE_EQUAL(1, c.numberOfCalls);
   //
   voidZeroArgMemberFunctionCaller.CallNonConstMemberFunction(&c, &NonConstClass::NonConstMemberVoidFunction);
   //
   ARE_EQUAL(2, c.numberOfCalls);
}

RUN_TESTS(VoidZeroArgMemberFunctionCallerTests)
