#include "pch.h"
#include "libFileRevisor/Components/Iteration/ForEach/OneExtraArgMemberForEacher.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/MetalMock/ParallelOneExtraArgMemberForEacherMock.h"

template<typename ElementType, typename ExtraArgType>
TEMPLATE_TESTS(ParallelOneExtraArgMemberForEacherTests, ElementType, ExtraArgType)
AFACT(ParallelOneExtraArgMemberForEach_EmptyElements_DoesNotCallMemberFunction)
AFACT(ParallelOneExtraArgMemberForEach_OneItemElements_CallsMemberFunctionWithElementAndExtraArgOnce)
AFACT(ParallelOneExtraArgMemberForEach_TwoItemElements_CallsInParallelOnBothElementsTheMemberFunctionWithElementAndExtraArg)
AFACT(TestingClassTwoArgMemberFunction_DoesNothing)
EVIDENCE

class TestingClass
{
public:
   virtual void TwoArgMemberFunction(ElementType, ExtraArgType) const {}
   virtual ~TestingClass() = default;
};

class TestingClassMock : public Metal::Mock<TestingClass>
{
public:
   vector<ElementType> elements;
   METALMOCK_VOID2_CONST(TwoArgMemberFunction, ElementType, ExtraArgType)
};

using ParallelOneExtraArgMemberForEacherType = ParallelOneExtraArgMemberForEacher<
   ElementType, TestingClass, void (TestingClass::*)(ElementType, ExtraArgType) const, ExtraArgType>;

ParallelOneExtraArgMemberForEacherType _parallelOneExtraArgMemberForEacher_DeleteDirectory;

TEST(ParallelOneExtraArgMemberForEach_EmptyElements_DoesNotCallMemberFunction)
{
   const TestingClassMock testingClassMock{};
   const ExtraArgType extraArg = ZenUnit::Random<ExtraArgType>();
   //
   _parallelOneExtraArgMemberForEacher_DeleteDirectory.ParallelOneExtraArgMemberForEach(
      testingClassMock.elements, &testingClassMock, &TestingClass::TwoArgMemberFunction, extraArg);
}

TEST(ParallelOneExtraArgMemberForEach_OneItemElements_CallsMemberFunctionWithElementAndExtraArgOnce)
{
   TestingClassMock testingClassMock{};
   testingClassMock.elements = { ZenUnit::Random<ElementType>() };
   testingClassMock.TwoArgMemberFunctionMock.Expect();
   const ExtraArgType extraArg = ZenUnit::Random<ExtraArgType>();
   //
   _parallelOneExtraArgMemberForEacher_DeleteDirectory.ParallelOneExtraArgMemberForEach(
      testingClassMock.elements, &testingClassMock, &TestingClass::TwoArgMemberFunction, extraArg);
   //
   METALMOCK(testingClassMock.TwoArgMemberFunctionMock.CalledOnceWith(testingClassMock.elements[0], extraArg));
}

TEST(ParallelOneExtraArgMemberForEach_TwoItemElements_CallsInParallelOnBothElementsTheMemberFunctionWithElementAndExtraArg)
{
   //TestingClassMock testingClassMock;
   //testingClassMock.elements = { ZenUnit::Random<ElementType>(), ZenUnit::Random<ElementType>() };
   //testingClassMock.TwoArgMemberFunctionMock.Expect();
   //const ExtraArgType extraArg = ZenUnit::Random<ExtraArgType>();
   ////
   //_parallelOneExtraArgMemberForEacher_DeleteDirectory.ParallelOneExtraArgMemberForEach(
   //   testingClassMock.elements, &testingClassMock, &TestingClass::TwoArgMemberFunction, extraArg);
   ////
   //vector<MetalMock::TwoArgumentFunctionCall<ElementType, ExtraArgType>> expectedMetalMockedFunctionCallHistory =
   //{
   //   { testingClassMock.elements[0], extraArg },
   //   { testingClassMock.elements[1], extraArg }
   //};
   //INDEXABLES_ARE_EQUAL_IN_ANY_ORDER(
   //   expectedMetalMockedFunctionCallHistory,
   //   testingClassMock.TwoArgMemberFunctionMock.metalMockedFunctionCallHistory);

   //// To prevent Fatal Expected But Not Asserted MetalMocked Function
   //testingClassMock.TwoArgMemberFunctionMock._wasAsserted = true;
}

TEST(TestingClassTwoArgMemberFunction_DoesNothing)
{
   const TestingClass testingClass{};
   testingClass.TwoArgMemberFunction(ZenUnit::Random<ElementType>(), ZenUnit::Random<ExtraArgType>());
}

RUN_TEMPLATE_TESTS(ParallelOneExtraArgMemberForEacherTests, int, int)
THEN_RUN_TEMPLATE_TESTS(ParallelOneExtraArgMemberForEacherTests, double, char)
