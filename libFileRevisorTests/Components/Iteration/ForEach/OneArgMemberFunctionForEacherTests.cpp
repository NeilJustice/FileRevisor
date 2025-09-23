#include "pch.h"
#include "libFileRevisor/Components/Iteration/ForEach/OneArgMemberFunctionForEacher.h"

template<typename T>
TEMPLATE_TESTS(OneArgMemberFunctionForEacherTests, T)
AFACT(TwoArgMemberForEach_EmptyCollection_DoesNotCallFunc)
AFACT(TwoArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
AFACT(TwoArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
AFACT(TwoArgFunction_CodeCoverage)
EVIDENCE

class ClassType
{
public:
   virtual void TwoArgFunction(const T&) const {}
   virtual ~ClassType() = default;
};

class ClassTypeMock : public Metal::Mock<ClassType>
{
public:
   vector<T> elements;
   METALMOCK_VOID1_CONST(TwoArgFunction, const T&)
};

using OneArgMemberFunctionForEacherType = OneArgMemberFunctionForEacher<ClassType, T>;

OneArgMemberFunctionForEacherType _oneArgMemberFunctionForEacher;

TEST(TwoArgMemberForEach_EmptyCollection_DoesNotCallFunc)
{
   const ClassTypeMock classInstance{};
   //
   _oneArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::TwoArgFunction);
}

TEST(TwoArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
{
   ClassTypeMock classInstance{};
   const T element1 = ZenUnit::Random<T>();
   classInstance.elements = { element1 };
   classInstance.TwoArgFunctionMock.Expect();
   //
   _oneArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::TwoArgFunction);
   //
   classInstance.TwoArgFunctionMock.CalledOnceWith(element1);
}

TEST(TwoArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
{
   ClassTypeMock classInstance{};
   const T element1 = ZenUnit::Random<T>();
   const T element2 = ZenUnit::Random<T>();
   classInstance.elements = { element1, element2 };
   classInstance.TwoArgFunctionMock.Expect();
   //
   _oneArgMemberFunctionForEacher.CallConstMemberFunctionWithEachElement(
      classInstance.elements, &classInstance, &ClassType::TwoArgFunction);
   //
   METALMOCK(classInstance.TwoArgFunctionMock.CalledNTimes(2));
   METALMOCKTHEN(classInstance.TwoArgFunctionMock.CalledWith(element1)).Then(
   METALMOCKTHEN(classInstance.TwoArgFunctionMock.CalledWith(element2)));
}

TEST(TwoArgFunction_CodeCoverage)
{
   const ClassType classInstance{};
   classInstance.TwoArgFunction(T{});
}

RUN_TEMPLATE_TESTS(OneArgMemberFunctionForEacherTests, int)
THEN_RUN_TEMPLATE_TESTS(OneArgMemberFunctionForEacherTests, string)
