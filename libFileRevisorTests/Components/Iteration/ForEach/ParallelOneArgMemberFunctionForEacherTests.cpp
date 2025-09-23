#include "pch.h"
#include "libFileRevisor/Components/Iteration/ForEach/ParallelOneArgMemberFunctionForEacher.h"

template<typename T>
TEMPLATE_TESTS(ParallelOneArgMemberFunctionForEacherTests, T)
AFACT(ParallelCallConstMemberFunctionWithEachElement_EmptyElements_DoesNotCallMemberFunction)
AFACT(ParallelCallConstMemberFunctionWithEachElement_OneItemElements_CallsMemberFunctionWithElementAndExtraArgOnce)
AFACT(ParallelCallConstMemberFunctionWithEachElement_TwoItemElements_CallsInParallelOnBothElementsTheMemberFunctionWithElementAndExtraArg)
AFACT(TestingClassOneArgMemberFunction_CodeCoverage)
EVIDENCE

class TestingClass
{
public:
   mutable vector<T> functionArguments;
   mutable mutex functionArgumentsMutex;

   virtual void OneArgMemberFunction(const T& element) const
   {
      scoped_lock<mutex> functionArgumentsLock(functionArgumentsMutex);
      functionArguments.emplace_back(element);
   }

   void AssertFunctionNotCalled() const
   {
      IS_EMPTY(functionArguments);
   }

   void AssertFunctionCalledOnceWith(const T& expectedElement) const
   {
      ARE_EQUAL(1, functionArguments.size());
      ARE_EQUAL(expectedElement, functionArguments[0]);
   }

   void AssertCalledTwiceInAnyOrder(const T& expectedElementA, const T& expectedElementB) const
   {
      const vector<T> expectedFunctionArguments =
      {
         { expectedElementA },
         { expectedElementB }
      };
      INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedFunctionArguments, functionArguments);
   }

   virtual ~TestingClass() = default;
};

using ConstMemberFunctionType = void(TestingClass::*)(const T&) const;
using ParallelOneArgMemberFunctionForEacherType = ParallelOneArgMemberFunctionForEacher<TestingClass, T>;

ParallelOneArgMemberFunctionForEacherType _parallelOneArgMemberFunctionForEacher_DeleteDirectory;

TEST(ParallelCallConstMemberFunctionWithEachElement_EmptyElements_DoesNotCallMemberFunction)
{
   const TestingClass testingClass{};
   const vector<T> emptyElements;
   //
   _parallelOneArgMemberFunctionForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
      emptyElements, &testingClass, &TestingClass::OneArgMemberFunction);
   //
   testingClass.AssertFunctionNotCalled();
}

TEST(ParallelCallConstMemberFunctionWithEachElement_OneItemElements_CallsMemberFunctionWithElementAndExtraArgOnce)
{
   TestingClass testingClass{};
   const vector<T> elements = { ZenUnit::Random<T>() };
   //
   _parallelOneArgMemberFunctionForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
      elements, &testingClass, &TestingClass::OneArgMemberFunction);
   //
   testingClass.AssertFunctionCalledOnceWith(elements[0]);
}

TEST(ParallelCallConstMemberFunctionWithEachElement_TwoItemElements_CallsInParallelOnBothElementsTheMemberFunctionWithElementAndExtraArg)
{
   TestingClass testingClass{};
   const vector<T> elements = { ZenUnit::Random<T>(), ZenUnit::Random<T>() };
   //
   _parallelOneArgMemberFunctionForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
      elements, &testingClass, &TestingClass::OneArgMemberFunction);
   //
   testingClass.AssertCalledTwiceInAnyOrder(elements[0], elements[1]);
}

TEST(TestingClassOneArgMemberFunction_CodeCoverage)
{
   const TestingClass testingClass{};
   testingClass.OneArgMemberFunction(ZenUnit::Random<T>());
}

RUN_TEMPLATE_TESTS(ParallelOneArgMemberFunctionForEacherTests, int)
THEN_RUN_TEMPLATE_TESTS(ParallelOneArgMemberFunctionForEacherTests, string)
