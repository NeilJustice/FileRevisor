#include "pch.h"
#include "libFileRevisor/Components/Iteration/ForEach/FourArgMemberFunctionForEacher.h"

template<typename ElementType, typename Arg2Type, typename Arg3Type, typename Arg4Type>
TEMPLATE_TESTS(FourArgMemberFunctionForEacherTests, ElementType, Arg2Type, Arg3Type, Arg4Type)
AFACT(CallConstMemberFunctionWithEachElement_EmptyCollection_DoesNotCallFunction)
AFACT(CallConstMemberFunctionWithEachElement_OneItemCollection_CallsThisPointerBoundFunctionOnce)
AFACT(CallConstMemberFunctionWithEachElement_TwoItemCollection_CallsThisPointerBoundFunctionTwice)
AFACT(CallNonConstMemberFunctionWithEachElement_EmptyCollection_DoesNotCallFunction)
AFACT(CallNonConstMemberFunctionWithEachElement_OneItemCollection_CallsThisPointerBoundFunctionOnce)
AFACT(CallNonConstMemberFunctionWithEachElement_TwoItemCollection_CallsThisPointerBoundFunctionTwice)
AFACT(ConstAndNonConstFourArgFunction_CodeCoverage)
EVIDENCE

class ClassType
{
public:
   virtual void ConstFourArgFunction(const ElementType&, Arg2Type, Arg3Type, Arg4Type) const {}
   virtual void NonConstFourArgFunction(const ElementType&, Arg2Type, Arg3Type, Arg4Type) {}
   virtual ~ClassType() = default;
};

class ClassTypeMock : public Metal::Mock<ClassType>
{
public:
   vector<ElementType> elements;
   METALMOCK_VOID4_CONST(ConstFourArgFunction, const ElementType&, Arg2Type, Arg3Type, Arg4Type)
   METALMOCK_VOID4(NonConstFourArgFunction, const ElementType&, Arg2Type, Arg3Type, Arg4Type)
};

using FourArgMemberFunctionForEacherType = Utils::FourArgMemberFunctionForEacher<ClassType, ElementType, Arg2Type, Arg3Type, Arg4Type>;

FourArgMemberFunctionForEacherType _fourArgMemberFunctionForEacher;

TEST(CallConstMemberFunctionWithEachElement_EmptyCollection_DoesNotCallFunction)
{
   const ClassTypeMock classInstance{};
   _fourArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::ConstFourArgFunction, 0, 0, 0);
}

TEST(CallConstMemberFunctionWithEachElement_OneItemCollection_CallsThisPointerBoundFunctionOnce)
{
   ClassTypeMock classInstance{};
   classInstance.elements = { 1 };
   classInstance.ConstFourArgFunctionMock.Expect();
   //
   _fourArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::ConstFourArgFunction, 10, 20, 30);
   //
   classInstance.ConstFourArgFunctionMock.CalledOnceWith(1, 10, 20, 30);
}

TEST(CallConstMemberFunctionWithEachElement_TwoItemCollection_CallsThisPointerBoundFunctionTwice)
{
   ClassTypeMock classInstance{};
   classInstance.elements = { 1, 2 };
   classInstance.ConstFourArgFunctionMock.Expect();
   //
   _fourArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::ConstFourArgFunction, 20, 30, 40);
   //
   METALMOCK(classInstance.ConstFourArgFunctionMock.CalledNTimes(2));
   METALMOCKTHEN(classInstance.ConstFourArgFunctionMock.CalledWith(1, 20, 30, 40)).Then(
   METALMOCKTHEN(classInstance.ConstFourArgFunctionMock.CalledWith(2, 20, 30, 40)));
}

TEST(CallNonConstMemberFunctionWithEachElement_EmptyCollection_DoesNotCallFunction)
{
   ClassTypeMock classInstance{};
   _fourArgMemberFunctionForEacher.CallNonConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::NonConstFourArgFunction, 0, 0, 0);
}

TEST(CallNonConstMemberFunctionWithEachElement_OneItemCollection_CallsThisPointerBoundFunctionOnce)
{
   ClassTypeMock classInstance{};
   classInstance.elements = { 1 };
   classInstance.NonConstFourArgFunctionMock.Expect();
   //
   _fourArgMemberFunctionForEacher.CallNonConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::NonConstFourArgFunction, 10, 20, 30);
   //
   classInstance.NonConstFourArgFunctionMock.CalledOnceWith(1, 10, 20, 30);
}

TEST(CallNonConstMemberFunctionWithEachElement_TwoItemCollection_CallsThisPointerBoundFunctionTwice)
{
   ClassTypeMock classInstance{};
   classInstance.elements = { 1, 2 };
   classInstance.NonConstFourArgFunctionMock.Expect();
   //
   _fourArgMemberFunctionForEacher.CallNonConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::NonConstFourArgFunction, 20, 30, 40);
   //
   METALMOCK(classInstance.NonConstFourArgFunctionMock.CalledNTimes(2));
   METALMOCKTHEN(classInstance.NonConstFourArgFunctionMock.CalledWith(1, 20, 30, 40)).Then(
   METALMOCKTHEN(classInstance.NonConstFourArgFunctionMock.CalledWith(2, 20, 30, 40)));
}

TEST(ConstAndNonConstFourArgFunction_CodeCoverage)
{
   ClassType classType{};
   classType.ConstFourArgFunction(ElementType{}, Arg2Type{}, Arg3Type{}, Arg4Type{});
   classType.NonConstFourArgFunction(ElementType{}, Arg2Type{}, Arg3Type{}, Arg4Type{});
};

RUN_TEMPLATE_TESTS(FourArgMemberFunctionForEacherTests, int, int, int, int)
THEN_RUN_TEMPLATE_TESTS(FourArgMemberFunctionForEacherTests, double, int, unsigned, unsigned long long)
