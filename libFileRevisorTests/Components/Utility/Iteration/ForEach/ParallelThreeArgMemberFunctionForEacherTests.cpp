#include "pch.h"
#include "libFileRevisor/Components/Utility/Iteration/ForEach/ThreeArgMemberFunctionForEacher.h"
#include "libFileRevisorTests/Components/Utility/Iteration/ForEach/MetalMock/ParallelThreeArgMemberFunctionForEacherMock.h"

template<typename T, typename Arg2Type, typename Arg3Type>
TEMPLATE_TESTS(ParallelThreeArgMemberFunctionForEacherTests, T, Arg2Type, Arg3Type)
AFACT(ParallelCallConstMemberFunctionWithEachElement_EmptyElements_DoesNotCallMemberFunction)
AFACT(ParallelCallConstMemberFunctionWithEachElement_OneItemElements_CallsMemberFunctionWithElementAndExtraArgOnce)
AFACT(ParallelCallConstMemberFunctionWithEachElement_TwoItemElements_CallsInParallelOnBothElementsTheMemberFunctionWithElementAndExtraArg)
AFACT(TestingClassTwoArgMemberFunction_CodeCoverage)
EVIDENCE

class TestingClass
{
public:
   mutable vector<tuple<T, Arg2Type, Arg3Type>> functionArguments;
   mutable mutex functionArgumentsMutex;

   virtual void TwoArgMemberFunction(const T& element, Arg2Type arg2, Arg3Type arg3) const
   {
      scoped_lock<mutex> functionArgumentsLock(functionArgumentsMutex);
      functionArguments.emplace_back(element, arg2, arg3);
   }

   void AssertFunctionNotCalled() const
   {
      IS_EMPTY(functionArguments);
   }

   void AssertFunctionCalledOnceWith(const T& expectedElement, const Arg2Type& expectedArg2, const Arg3Type& expectedArg3) const
   {
      ARE_EQUAL(1, functionArguments.size());
      ARE_EQUAL(expectedElement, get<0>(functionArguments[0]));
      ARE_EQUAL(expectedArg2, get<1>(functionArguments[0]));
      ARE_EQUAL(expectedArg3, get<2>(functionArguments[0]));
   }

   void AssertCalledTwiceInAnyOrder(
      const T& expectedElementA, const Arg2Type& expectedArg2A, const Arg3Type& expectedArg3A,
      const T& expectedElementB, const Arg2Type& expectedArg2B, const Arg3Type& expectedArg3B) const
   {
      const vector<tuple<T, Arg2Type, Arg3Type>> expectedFunctionArguments =
      {
         { expectedElementA, expectedArg2A, expectedArg3A },
         { expectedElementB, expectedArg2B, expectedArg3B }
      };
      INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(expectedFunctionArguments, functionArguments);
   }

   virtual ~TestingClass() = default;
};

using ConstMemberFunctionType = void(TestingClass::*)(const T&, Arg2Type, Arg3Type) const;
using ParallelThreeArgMemberFunctionForEacherType = ParallelThreeArgMemberFunctionForEacher<TestingClass, T, Arg2Type, Arg3Type>;

ParallelThreeArgMemberFunctionForEacherType _parallelThreeArgMemberFunctionForEacher_DeleteDirectory;

TEST(ParallelCallConstMemberFunctionWithEachElement_EmptyElements_DoesNotCallMemberFunction)
{
   const TestingClass testingClass{};
   const vector<T> emptyElements;
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   const Arg3Type arg3 = ZenUnit::Random<Arg3Type>();
   //
   _parallelThreeArgMemberFunctionForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
      emptyElements, &testingClass, &TestingClass::TwoArgMemberFunction, arg2, arg3);
   //
   testingClass.AssertFunctionNotCalled();
}

TEST(ParallelCallConstMemberFunctionWithEachElement_OneItemElements_CallsMemberFunctionWithElementAndExtraArgOnce)
{
   TestingClass testingClass{};
   const vector<T> elements = { ZenUnit::Random<T>() };
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   const Arg3Type arg3 = ZenUnit::Random<Arg3Type>();
   //
   _parallelThreeArgMemberFunctionForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
      elements, &testingClass, &TestingClass::TwoArgMemberFunction, arg2, arg3);
   //
   testingClass.AssertFunctionCalledOnceWith(elements[0], arg2, arg3);
}

TEST(ParallelCallConstMemberFunctionWithEachElement_TwoItemElements_CallsInParallelOnBothElementsTheMemberFunctionWithElementAndExtraArg)
{
   TestingClass testingClass{};
   const vector<T> elements = { ZenUnit::Random<T>(), ZenUnit::Random<T>() };
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   const Arg3Type arg3 = ZenUnit::Random<Arg3Type>();
   //
   _parallelThreeArgMemberFunctionForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
      elements, &testingClass, &TestingClass::TwoArgMemberFunction, arg2, arg3);
   //
   testingClass.AssertCalledTwiceInAnyOrder(
      elements[0], arg2, arg3,
      elements[1], arg2, arg3);
}

TEST(TestingClassTwoArgMemberFunction_CodeCoverage)
{
   const TestingClass testingClass{};
   testingClass.TwoArgMemberFunction(ZenUnit::Random<T>(), ZenUnit::Random<Arg2Type>(), ZenUnit::Random<Arg3Type>());
}

RUN_TEMPLATE_TESTS(ParallelThreeArgMemberFunctionForEacherTests, int, int, int)
THEN_RUN_TEMPLATE_TESTS(ParallelThreeArgMemberFunctionForEacherTests, double, char, string)
