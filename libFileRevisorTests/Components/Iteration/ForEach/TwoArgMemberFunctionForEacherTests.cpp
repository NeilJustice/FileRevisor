#include "pch.h"
#include "libFileRevisor/Components/Iteration/ForEach/TwoArgMemberFunctionForEacher.h"

template<typename T, typename Arg2Type>
TEMPLATE_TESTS(TwoArgMemberFunctionForEacherTests, T, Arg2Type)
AFACT(TwoArgMemberForEach_EmptyCollection_DoesNotCallFunc)
AFACT(TwoArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
AFACT(TwoArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
AFACT(TwoArgFunction_CodeCoverage)
EVIDENCE

class ClassType
{
public:
   virtual void TwoArgFunction(const T&, Arg2Type) const {}
   virtual ~ClassType() = default;
};

class ClassTypeMock : public Metal::Mock<ClassType>
{
public:
   vector<T> elements;
   METALMOCK_VOID2_CONST(TwoArgFunction, const T&, Arg2Type)
};

using TwoArgMemberFunctionForEacherType = TwoArgMemberFunctionForEacher<ClassType, T, Arg2Type>;

TwoArgMemberFunctionForEacherType _twoArgMemberFunctionForEacher;

TEST(TwoArgMemberForEach_EmptyCollection_DoesNotCallFunc)
{
   const ClassTypeMock classInstance{};
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _twoArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::TwoArgFunction, arg2);
}

TEST(TwoArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
{
   ClassTypeMock classInstance{};
   const T element1 = ZenUnit::Random<T>();
   classInstance.elements = { element1 };
   classInstance.TwoArgFunctionMock.Expect();
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _twoArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::TwoArgFunction, arg2);
   //
   classInstance.TwoArgFunctionMock.CalledOnceWith(element1, arg2);
}

TEST(TwoArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
{
   ClassTypeMock classInstance{};
   const T element1 = ZenUnit::Random<T>();
   const T element2 = ZenUnit::Random<T>();
   classInstance.elements = { element1, element2 };
   classInstance.TwoArgFunctionMock.Expect();
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _twoArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::TwoArgFunction, arg2);
   //
   METALMOCK(classInstance.TwoArgFunctionMock.CalledNTimes(2));
   METALMOCKTHEN(classInstance.TwoArgFunctionMock.CalledWith(element1, arg2)).Then(
   METALMOCKTHEN(classInstance.TwoArgFunctionMock.CalledWith(element2, arg2)));
}

TEST(TwoArgFunction_CodeCoverage)
{
   ClassType classInstance{};
   classInstance.TwoArgFunction(T{}, Arg2Type{});
}

RUN_TEMPLATE_TESTS(TwoArgMemberFunctionForEacherTests, int, int)
THEN_RUN_TEMPLATE_TESTS(TwoArgMemberFunctionForEacherTests, double, char)
