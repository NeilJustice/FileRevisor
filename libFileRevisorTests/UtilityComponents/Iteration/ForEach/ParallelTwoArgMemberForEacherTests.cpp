#include "pch.h"
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/TwoArgMemberForEacher.h"
#include "libFileRevisorTests/UtilityComponents/Iteration/ForEach/MetalMock/ParallelTwoArgMemberForEacherMock.h"

template<typename T, typename Arg2Type>
TEMPLATE_TESTS(ParallelTwoArgMemberForEacherTests, T, Arg2Type)
AFACT(ParallelCallConstMemberFunctionWithEachElement_EmptyElements_DoesNotCallMemberFunction)
AFACT(ParallelCallConstMemberFunctionWithEachElement_OneItemElements_CallsMemberFunctionWithElementAndExtraArgOnce)
//AFACT(ParallelCallConstMemberFunctionWithEachElement_TwoItemElements_CallsInParallelOnBothElementsTheMemberFunctionWithElementAndExtraArg)
AFACT(TestingClassTwoArgMemberFunction_CodeCoverage)
EVIDENCE

class TestingClass
{
public:
   virtual void TwoArgMemberFunction(const T&, Arg2Type) const {}
   virtual ~TestingClass() = default;
};

class TestingClassMock : public Metal::Mock<TestingClass>
{
public:
   vector<T> elements;
   METALMOCK_VOID2_CONST(TwoArgMemberFunction, const T&, Arg2Type)
};

using ConstMemberFunctionType = void(TestingClass::*)(const T&, Arg2Type) const;
using ParallelTwoArgMemberForEacherType = ParallelTwoArgMemberForEacher<TestingClass, T, Arg2Type>;

ParallelTwoArgMemberForEacherType _parallelTwoArgMemberForEacher_DeleteDirectory;

TEST(ParallelCallConstMemberFunctionWithEachElement_EmptyElements_DoesNotCallMemberFunction)
{
   const TestingClassMock testingClassMock{};
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _parallelTwoArgMemberForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
      testingClassMock.elements, &testingClassMock, &TestingClass::TwoArgMemberFunction, arg2);
}

TEST(ParallelCallConstMemberFunctionWithEachElement_OneItemElements_CallsMemberFunctionWithElementAndExtraArgOnce)
{
   TestingClassMock testingClassMock{};
   testingClassMock.elements = { ZenUnit::Random<T>() };
   testingClassMock.TwoArgMemberFunctionMock.Expect();
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _parallelTwoArgMemberForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
      testingClassMock.elements, &testingClassMock, &TestingClass::TwoArgMemberFunction, arg2);
   //
   METALMOCK(testingClassMock.TwoArgMemberFunctionMock.CalledOnceWith(testingClassMock.elements[0], arg2));
}

// TEST(ParallelCallConstMemberFunctionWithEachElement_TwoItemElements_CallsInParallelOnBothElementsTheMemberFunctionWithElementAndExtraArg)
// {
//    TestingClassMock testingClassMock{};
//    testingClassMock.elements = { ZenUnit::Random<T>(), ZenUnit::Random<T>() };
//    testingClassMock.TwoArgMemberFunctionMock.Expect();
//    const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
//    //
//    _parallelTwoArgMemberForEacher_DeleteDirectory.ParallelCallConstMemberFunctionWithEachElement(
//       testingClassMock.elements, &testingClassMock, &TestingClass::TwoArgMemberFunction, arg2);
//    //
//    METALMOCK(testingClassMock.TwoArgMemberFunctionMock.CalledAsFollowsInAnyOrder(
//    {
//       { testingClassMock.elements[0], arg2 },
//       { testingClassMock.elements[1], arg2 }
//    }));
// }

TEST(TestingClassTwoArgMemberFunction_CodeCoverage)
{
   const TestingClass testingClass{};
   testingClass.TwoArgMemberFunction(ZenUnit::Random<T>(), ZenUnit::Random<Arg2Type>());
}

RUN_TEMPLATE_TESTS(ParallelTwoArgMemberForEacherTests, int, int)
THEN_RUN_TEMPLATE_TESTS(ParallelTwoArgMemberForEacherTests, double, char)
