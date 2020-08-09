#include "pch.h"
#include "libFileRevisor/Components/Iteration/ForEach/OneExtraArgMemberForEacher.h"

template<typename ElementType, typename Arg2Type>
TEMPLATE_TESTS(OneExtraArgMemberForEacherTests, ElementType, Arg2Type)
AFACT(TwoArgMemberForEach_EmptyCollection_DoesNotCallFunc)
AFACT(TwoArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
AFACT(TwoArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
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

class ClassTypeMock : public Metal::Mock<ClassType>
{
public:
   vector<ElementType> vec;
   METALMOCK_VOID2_CONST(TwoArgFunction, ElementType, Arg2Type)
};

using OneExtraArgMemberForEacherType = OneExtraArgMemberForEacher<
   ElementType, ClassType, void (ClassType::*)(ElementType, Arg2Type) const, Arg2Type>;

OneExtraArgMemberForEacherType _oneExtraArgMemberForEacher_DeleteDirectory;

TEST(TwoArgMemberForEach_EmptyCollection_DoesNotCallFunc)
{
   const ClassTypeMock classInstance{};
   _oneExtraArgMemberForEacher_DeleteDirectory.OneExtraArgMemberForEach(classInstance.vec, &classInstance, &ClassType::TwoArgFunction, 0);
}

TEST(TwoArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
{
   ClassTypeMock classInstance;
   classInstance.vec = { 1 };
   classInstance.TwoArgFunctionMock.Expect();
   //
   _oneExtraArgMemberForEacher_DeleteDirectory.OneExtraArgMemberForEach(classInstance.vec, &classInstance, &ClassType::TwoArgFunction, 10);
   //
   classInstance.TwoArgFunctionMock.CalledOnceWith(1, 10);
}

TEST(TwoArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
{
   ClassTypeMock classInstance;
   classInstance.vec = { 1, 2 };
   classInstance.TwoArgFunctionMock.Expect();
   //
   _oneExtraArgMemberForEacher_DeleteDirectory.OneExtraArgMemberForEach(classInstance.vec, &classInstance, &ClassType::TwoArgFunction, 20);
   //
   classInstance.TwoArgFunctionMock.CalledAsFollows(
   {
      { 1, 20 },
      { 2, 20 }
   });
}

TEST(TwoArgFunction_CodeCoverage)
{
   ClassType classInstance;
   classInstance.TwoArgFunction(ElementType{}, Arg2Type{});
}

RUN_TEMPLATE_TESTS(OneExtraArgMemberForEacherTests, int, int)
THEN_RUN_TEMPLATE_TESTS(OneExtraArgMemberForEacherTests, double, char)
