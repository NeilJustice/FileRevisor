#include "pch.h"
#include "libFileRevisor/Components/Utility/Iteration/ForEach/ParallelTwoArgMemberFunctionForEacher.h"

template<typename T, typename Arg2Type>
TEMPLATE_TESTS(ParallelTwoArgMemberFunctionForEacherTests, T, Arg2Type)
AFACT(ParallelCallConstMemberFunctionWithEachElement_EmptyElements_DoesNotCallMemberFunction)
AFACT(ParallelCallConstMemberFunctionWithEachElement_OneItemElements_CallsMemberFunctionWithElementAndExtraArgOnce)
AFACT(ParallelCallConstMemberFunctionWithEachElement_TwoItemElements_CallsInParallelOnBothElementsTheMemberFunctionWithElementAndExtraArg)
AFACT(TestingClassTwoArgMemberFunction_CodeCoverage)
EVIDENCE

class TestingClass
{
public:
   mutable vector<pair<T, Arg2Type>> functionArguments;
   mutable mutex functionArgumentsMutex;

   virtual void TwoArgMemberFunction(const T& element, Arg2Type arg2) const
   {
      scoped_lock<mutex> functionArgumentsLock(functionArgumentsMutex);
      functionArguments.emplace_back(element, arg2);
   }

   void AssertFunctionNotCalled() const
   {
      IS_EMPTY(functionArguments);
   }

   void AssertFunctionCalledOnceWith(const T& expectedElement, const Arg2Type& expectedArg2) const
   {
      ARE_EQUAL(1, functionArguments.size());
      ARE_EQUAL(expectedElement, functionArguments[0].first);
      ARE_EQUAL(expectedArg2, functionArguments[0].second);
   }

   void AssertCalledTwiceInAnyOrder(
      const T& expectedElementA, const Arg2Type& expectedArg2A,
      const T& expectedElementB, const Arg2Type& expectedArg2B) const
   {
      const vector<pair<T, Arg2Type>> expectedFunctionArguments =
      {
         { expectedElementA, expectedArg2A },
         { expectedElementB, expectedArg2B }
      };
      INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedFunctionArguments, functionArguments);
   }

   virtual ~TestingClass() = default;
};

using ConstMemberFunctionType = void(TestingClass::*)(const T&, Arg2Type) const;
using ParallelTwoArgMemberFunctionForEacherType = ParallelTwoArgMemberFunctionForEacher<TestingClass, T, Arg2Type>;

ParallelTwoArgMemberFunctionForEacherType _parallelTwoArgMemberFunctionForEacher_DeleteDirectory;

TEST(ParallelCallConstMemberFunctionWithEachElement_EmptyElements_DoesNotCallMemberFunction)
{
   const TestingClass testingClass{};
   const vector<T> emptyElements;
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _parallelTwoArgMemberFunctionForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
      emptyElements, &testingClass, &TestingClass::TwoArgMemberFunction, arg2);
   //
   testingClass.AssertFunctionNotCalled();
}

TEST(ParallelCallConstMemberFunctionWithEachElement_OneItemElements_CallsMemberFunctionWithElementAndExtraArgOnce)
{
   TestingClass testingClass{};
   const vector<T> elements = { ZenUnit::Random<T>() };
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _parallelTwoArgMemberFunctionForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
      elements, &testingClass, &TestingClass::TwoArgMemberFunction, arg2);
   //
   testingClass.AssertFunctionCalledOnceWith(elements[0], arg2);
}

TEST(ParallelCallConstMemberFunctionWithEachElement_TwoItemElements_CallsInParallelOnBothElementsTheMemberFunctionWithElementAndExtraArg)
{
   TestingClass testingClass{};
   const vector<T> elements = { ZenUnit::Random<T>(), ZenUnit::Random<T>() };
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _parallelTwoArgMemberFunctionForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
      elements, &testingClass, &TestingClass::TwoArgMemberFunction, arg2);
   //
   testingClass.AssertCalledTwiceInAnyOrder(
      elements[0], arg2,
      elements[1], arg2);
}

TEST(TestingClassTwoArgMemberFunction_CodeCoverage)
{
   const TestingClass testingClass{};
   testingClass.TwoArgMemberFunction(ZenUnit::Random<T>(), ZenUnit::Random<Arg2Type>());
}

RUN_TEMPLATE_TESTS(ParallelTwoArgMemberFunctionForEacherTests, int, int)
THEN_RUN_TEMPLATE_TESTS(ParallelTwoArgMemberFunctionForEacherTests, double, char)
