#include "pch.h"
#include "libFileRevisor/Components/Iteration/ForEach/OneExtraArgMemberForEacher.h"

template<typename ElementType, typename Arg2Type>
TEMPLATE_TESTS(ParallelOneExtraArgMemberForEacherTests, ElementType, Arg2Type)
AFACT(ParallelOneExtraArgMemberForEach_EmptyCollection_DoesNotCallFunc)
AFACT(ParallelOneExtraArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
//AFACT(ParallelOneExtraArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
AFACT(TwoArgFunction_CodeCoverage)
EVIDENCE

class ClassType
{
public:
   // Rule of 5 to satisfy clang-tidy hicpp-special-member-functions
   ClassType() = default;
   ClassType(const ClassType&) = default;
   ClassType& operator=(const ClassType&) = default;
   ClassType(ClassType&&) noexcept = default;
   ClassType& operator=(ClassType&&) noexcept = default;

   virtual void TwoArgFunction(ElementType, Arg2Type) const {}
   virtual ~ClassType() = default;
};

class ClassTypeMock : public Zen::Mock<ClassType>
{
public:
   vector<ElementType> vec;
   METALMOCK_VOID2_CONST(TwoArgFunction, ElementType, Arg2Type)
};

using ParallelOneExtraArgMemberForEacherType = ParallelOneExtraArgMemberForEacher<
   ElementType, ClassType, void (ClassType::*)(ElementType, Arg2Type) const, Arg2Type>;

ParallelOneExtraArgMemberForEacherType _parallelOneExtraArgMemberForEacher_DeleteDirectory;

TEST(ParallelOneExtraArgMemberForEach_EmptyCollection_DoesNotCallFunc)
{
   const ClassTypeMock classInstanceMock{};
   const Arg2Type extraArg = ZenUnit::Random<Arg2Type>();
   _parallelOneExtraArgMemberForEacher_DeleteDirectory.ParallelOneExtraArgMemberForEach(
      classInstanceMock.vec, &classInstanceMock, &ClassType::TwoArgFunction, extraArg);
}

TEST(ParallelOneExtraArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
{
   ClassTypeMock classInstanceMock;
   classInstanceMock.vec = { 1 };
   classInstanceMock.TwoArgFunctionMock.Expect();
   const Arg2Type extraArg = ZenUnit::Random<Arg2Type>();
   //
   _parallelOneExtraArgMemberForEacher_DeleteDirectory.ParallelOneExtraArgMemberForEach(
      classInstanceMock.vec, &classInstanceMock, &ClassType::TwoArgFunction, extraArg);
   //
   METALMOCK(classInstanceMock.TwoArgFunctionMock.CalledOnceWith(1, extraArg));
}

 //TEST(ParallelOneExtraArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
 //{
 //   ClassTypeMock classInstanceMock;
 //   classInstanceMock.vec = { 1, 2 };
 //   classInstanceMock.TwoArgFunctionMock.Expect();
 //   const Arg2Type extraArg = ZenUnit::Random<Arg2Type>();
 //   //
 //   _parallelOneExtraArgMemberForEacher_DeleteDirectory.ParallelOneExtraArgMemberForEach(
 //      classInstanceMock.vec, &classInstanceMock, &ClassType::TwoArgFunction, extraArg);
 //   //
 //   ARE_EQUAL(2, classInstanceMock.TwoArgFunctionMock.zenMockedFunctionCallHistory.size());

 //   ARE_EQUAL(1, classInstanceMock.TwoArgFunctionMock.zenMockedFunctionCallHistory[0].firstArgument.value);
 //   ARE_EQUAL(extraArg, classInstanceMock.TwoArgFunctionMock.zenMockedFunctionCallHistory[0].secondArgument.value);

 //   ARE_EQUAL(2, classInstanceMock.TwoArgFunctionMock.zenMockedFunctionCallHistory[1].firstArgument.value);
 //   ARE_EQUAL(extraArg, classInstanceMock.TwoArgFunctionMock.zenMockedFunctionCallHistory[1].secondArgument.value);

 //   classInstanceMock.TwoArgFunctionMock._wasAsserted = true;
 //}

TEST(TwoArgFunction_CodeCoverage)
{
   ClassType classInstance;
   classInstance.TwoArgFunction(ElementType{}, Arg2Type{});
}

RUN_TEMPLATE_TESTS(ParallelOneExtraArgMemberForEacherTests, int, int)
THEN_RUN_TEMPLATE_TESTS(ParallelOneExtraArgMemberForEacherTests, double, char)
