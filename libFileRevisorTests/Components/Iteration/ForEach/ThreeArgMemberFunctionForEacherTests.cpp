#include "pch.h"
#include "libFileRevisor/Components/Iteration/ForEach/ThreeArgMemberFunctionForEacher.h"

template<typename T, typename Arg2Type, typename Arg3Type>
TEMPLATE_TESTS(ThreeArgMemberFunctionForEacherTests, T, Arg2Type, Arg3Type)
AFACT(ThreeArgMemberForEach_EmptyCollection_DoesNotCallFunction)
AFACT(ThreeArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
AFACT(ThreeArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
AFACT(ThreeArgFunction_CodeCoverage)
EVIDENCE

class ClassType
{
public:
   virtual void ThreeArgFunction(const T&, Arg2Type, Arg3Type) const {}
   virtual ~ClassType() = default;
};

class ClassTypeMock : public Metal::Mock<ClassType>
{
public:
   vector<T> elements;
   METALMOCK_VOID3_CONST(ThreeArgFunction, const T&, Arg2Type, Arg3Type)
};

using ThreeArgMemberFunctionForEacherType = ThreeArgMemberFunctionForEacher<ClassType, T, Arg2Type, Arg3Type>;

ThreeArgMemberFunctionForEacherType _threeArgMemberFunctionForEacher;

TEST(ThreeArgMemberForEach_EmptyCollection_DoesNotCallFunction)
{
   const ClassTypeMock classInstance{};
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   const Arg3Type arg3 = ZenUnit::Random<Arg3Type>();
   //
   _threeArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(classInstance.elements, &classInstance, &ClassType::ThreeArgFunction, arg2, arg3);
}

TEST(ThreeArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
{
   ClassTypeMock classInstance{};
   const T element = ZenUnit::Random<T>();
   classInstance.elements = { element };
   classInstance.ThreeArgFunctionMock.Expect();
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   const Arg3Type arg3 = ZenUnit::Random<Arg3Type>();
   //
   _threeArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(classInstance.elements, &classInstance, &ClassType::ThreeArgFunction, arg2, arg3);
   //
   classInstance.ThreeArgFunctionMock.CalledOnceWith(element, arg2, arg3);
}

TEST(ThreeArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
{
   ClassTypeMock classInstance{};
   const T element1 = ZenUnit::Random<T>();
   const T element2 = ZenUnit::Random<T>();
   classInstance.elements = { element1, element2 };
   classInstance.ThreeArgFunctionMock.Expect();
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   const Arg3Type arg3 = ZenUnit::Random<Arg3Type>();
   //
   _threeArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(classInstance.elements, &classInstance, &ClassType::ThreeArgFunction, arg2, arg3);
   //
   METALMOCK(classInstance.ThreeArgFunctionMock.CalledNTimes(2));
   METALMOCKTHEN(classInstance.ThreeArgFunctionMock.CalledWith(element1, arg2, arg3)).Then(
   METALMOCKTHEN(classInstance.ThreeArgFunctionMock.CalledWith(element2, arg2, arg3)));
}

TEST(ThreeArgFunction_CodeCoverage)
{
   ClassType classInstance{};
   classInstance.ThreeArgFunction(T{}, Arg2Type{}, Arg3Type{});
}

RUN_TEMPLATE_TESTS(ThreeArgMemberFunctionForEacherTests, int, int, int)
THEN_RUN_TEMPLATE_TESTS(ThreeArgMemberFunctionForEacherTests, double, string, int)
