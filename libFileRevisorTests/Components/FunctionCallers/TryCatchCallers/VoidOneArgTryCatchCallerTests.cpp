#include "pch.h"
#include "libFileRevisor/Components/FunctionCallers/TryCatchCallers/VoidOneArgTryCatchCaller.h"

template<typename ExceptionType, typename Arg1Type>
TEMPLATE_TESTS(VoidOneArgTryCatchCallerTests, ExceptionType, Arg1Type)
AFACT(TryCatchCallConstMemberFunction_CallsConstMemberFunctionWhichDoesNotThrowException_Returns)
AFACT(TryCatchCallConstMemberFunction_CallsConstMemberFunctionWhichThrowsAnException_CallsExceptionHandlerWithExceptionClassNameAndMessage)
AFACT(TryCatchCallNonConstMemberFunction_CallsNonConstMemberFunctionWhichDoesNotThrowException_Returns)
AFACT(TryCatchCallNonConstMemberFunction_CallsNonConstMemberFunctionWhichThrowsAnException_CallsExceptionHandlerWithExceptionClassNameAndMessage)
EVIDENCE

class Class
{
public:
   bool doThrowException = false;
   mutable vector<Arg1Type> functionCallArguments;

   string exceptionMessage;
   mutable vector<string> exceptionHandlerArguments;

   Class()
      : exceptionMessage(ZenUnit::Random<string>())
   {
   }

   void ConstMemberFunction(Arg1Type arg1) const
   {
      functionCallArguments.emplace_back(arg1);
      if (doThrowException)
      {
         throw ExceptionType(exceptionMessage.c_str());
      }
   }

   void NonConstMemberFunction(Arg1Type arg1)
   {
      functionCallArguments.emplace_back(arg1);
      if (doThrowException)
      {
         throw ExceptionType(exceptionMessage.c_str());
      }
   }

   void ExceptionHandler(string_view exceptionClassNameAndMessage) const
   {
      exceptionHandlerArguments.emplace_back(exceptionClassNameAndMessage);
   }
};

Class _classInstance;
VoidOneArgTryCatchCaller<Class, Arg1Type> _voidOneArgTryCatchCaller;

TEST(TryCatchCallConstMemberFunction_CallsConstMemberFunctionWhichDoesNotThrowException_Returns)
{
   _classInstance.doThrowException = false;
   const Arg1Type arg1 = ZenUnit::Random<Arg1Type>();
   //
   _voidOneArgTryCatchCaller.TryCatchCallConstMemberFunction(
      &_classInstance, &Class::ConstMemberFunction, arg1, &Class::ExceptionHandler);
   //
   const vector<Arg1Type> expectedArguments =
   {
      arg1
   };
   VECTORS_ARE_EQUAL(expectedArguments, _classInstance.functionCallArguments);
   IS_EMPTY(_classInstance.exceptionHandlerArguments);
}

TEST(TryCatchCallConstMemberFunction_CallsConstMemberFunctionWhichThrowsAnException_CallsExceptionHandlerWithExceptionClassNameAndMessage)
{
   _classInstance.doThrowException = true;
   const Arg1Type arg1 = ZenUnit::Random<Arg1Type>();
   //
   _voidOneArgTryCatchCaller.TryCatchCallConstMemberFunction(
      &_classInstance, &Class::ConstMemberFunction, arg1, &Class::ExceptionHandler);
   //
   const vector<Arg1Type> expectedArguments =
   {
      arg1
   };
   VECTORS_ARE_EQUAL(expectedArguments, _classInstance.functionCallArguments);

   const ExceptionType expectedException(_classInstance.exceptionMessage.c_str());
   const string expectedExceptionClassNameAndMessage = Type::GetExceptionClassNameAndMessage(&expectedException);
   const vector<string> expectedExceptionHandlerArguments = { expectedExceptionClassNameAndMessage };
   VECTORS_ARE_EQUAL(expectedExceptionHandlerArguments, _classInstance.exceptionHandlerArguments);
}

TEST(TryCatchCallNonConstMemberFunction_CallsNonConstMemberFunctionWhichDoesNotThrowException_Returns)
{
   _classInstance.doThrowException = false;
   const Arg1Type arg1 = ZenUnit::Random<Arg1Type>();
   //
   _voidOneArgTryCatchCaller.TryCatchCallNonConstMemberFunction(
      &_classInstance, &Class::NonConstMemberFunction, arg1, &Class::ExceptionHandler);
   //
   const vector<Arg1Type> expectedArguments =
   {
      arg1
   };
   VECTORS_ARE_EQUAL(expectedArguments, _classInstance.functionCallArguments);
   IS_EMPTY(_classInstance.exceptionHandlerArguments);
}

TEST(TryCatchCallNonConstMemberFunction_CallsNonConstMemberFunctionWhichThrowsAnException_CallsExceptionHandlerWithExceptionClassNameAndMessage)
{
   _classInstance.doThrowException = true;
   const Arg1Type arg1 = ZenUnit::Random<Arg1Type>();
   //
   _voidOneArgTryCatchCaller.TryCatchCallNonConstMemberFunction(
      &_classInstance, &Class::NonConstMemberFunction,
      arg1,
      &Class::ExceptionHandler);
   //
   const vector<Arg1Type> expectedArguments =
   {
      arg1
   };
   VECTORS_ARE_EQUAL(expectedArguments, _classInstance.functionCallArguments);

   const ExceptionType expectedException(_classInstance.exceptionMessage.c_str());
   const string expectedExceptionClassNameAndMessage = Type::GetExceptionClassNameAndMessage(&expectedException);
   const vector<string> expectedExceptionHandlerArguments = { expectedExceptionClassNameAndMessage };
   VECTORS_ARE_EQUAL(expectedExceptionHandlerArguments, _classInstance.exceptionHandlerArguments);
}

RUN_TEMPLATE_TESTS(VoidOneArgTryCatchCallerTests, invalid_argument, string)
THEN_RUN_TEMPLATE_TESTS(VoidOneArgTryCatchCallerTests, runtime_error, fs::path)
