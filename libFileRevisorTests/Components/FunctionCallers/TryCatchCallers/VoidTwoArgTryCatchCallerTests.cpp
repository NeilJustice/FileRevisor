#include "pch.h"
#include "libFileRevisor/Components/FunctionCallers/TryCatchCallers/VoidTwoArgTryCatchCaller.h"

template<typename ExceptionType, typename Arg1Type, typename Arg2Type>
TEMPLATE_TESTS(VoidTwoArgTryCatchCallerTests, ExceptionType, Arg1Type, Arg2Type)
AFACT(TryCatchCallConstMemberFunction_CallsConstMemberFunctionWhichDoesNotThrowException_Returns)
AFACT(TryCatchCallConstMemberFunction_CallsConstMemberFunctionWhichThrowsAnException_CallsExceptionHandlerWithExceptionClassNameAndMessage)
AFACT(TryCatchCallNonConstMemberFunction_CallsNonConstMemberFunctionWhichDoesNotThrowException_Returns)
AFACT(TryCatchCallNonConstMemberFunction_CallsNonConstMemberFunctionWhichThrowsAnException_CallsExceptionHandlerWithExceptionClassNameAndMessage)
EVIDENCE

class Class
{
public:
   bool doThrowException = false;
   mutable vector<pair<Arg1Type, Arg2Type>> functionCallArguments;

   string exceptionMessage;
   mutable vector<string> exceptionHandlerArguments;

   Class()
      : exceptionMessage(ZenUnit::Random<string>())
   {
   }

   void ConstMemberFunction(Arg1Type arg1, Arg2Type arg2) const
   {
      functionCallArguments.emplace_back(arg1, arg2);
      if (doThrowException)
      {
         throw ExceptionType(exceptionMessage.c_str());
      }
   }

   void NonConstMemberFunction(Arg1Type arg1, Arg2Type arg2)
   {
      functionCallArguments.emplace_back(arg1, arg2);
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
VoidTwoArgTryCatchCaller<Class, Arg1Type, Arg2Type> _voidTwoArgTryCatchCaller;

TEST(TryCatchCallConstMemberFunction_CallsConstMemberFunctionWhichDoesNotThrowException_Returns)
{
   _classInstance.doThrowException = false;
   const Arg1Type arg1 = ZenUnit::Random<Arg1Type>();
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _voidTwoArgTryCatchCaller.TryCatchCallConstMemberFunction(&_classInstance, &Class::ConstMemberFunction, arg1, arg2, &Class::ExceptionHandler);
   //
   const vector<pair<Arg1Type, Arg2Type>> expectedArguments =
   {
      { arg1, arg2 }
   };
   VECTORS_ARE_EQUAL(expectedArguments, _classInstance.functionCallArguments);
   IS_EMPTY(_classInstance.exceptionHandlerArguments);
}

TEST(TryCatchCallConstMemberFunction_CallsConstMemberFunctionWhichThrowsAnException_CallsExceptionHandlerWithExceptionClassNameAndMessage)
{
   _classInstance.doThrowException = true;
   const Arg1Type arg1 = ZenUnit::Random<Arg1Type>();
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _voidTwoArgTryCatchCaller.TryCatchCallConstMemberFunction(&_classInstance, &Class::ConstMemberFunction, arg1, arg2, &Class::ExceptionHandler);
   //
   const vector<pair<Arg1Type, Arg2Type>> expectedArguments =
   {
      { arg1, arg2 }
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
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _voidTwoArgTryCatchCaller.TryCatchCallNonConstMemberFunction(&_classInstance, &Class::NonConstMemberFunction, arg1, arg2, &Class::ExceptionHandler);
   //
   const vector<pair<Arg1Type, Arg2Type>> expectedArguments =
   {
      { arg1, arg2 }
   };
   VECTORS_ARE_EQUAL(expectedArguments, _classInstance.functionCallArguments);
   IS_EMPTY(_classInstance.exceptionHandlerArguments);
}

TEST(TryCatchCallNonConstMemberFunction_CallsNonConstMemberFunctionWhichThrowsAnException_CallsExceptionHandlerWithExceptionClassNameAndMessage)
{
   _classInstance.doThrowException = true;
   const Arg1Type arg1 = ZenUnit::Random<Arg1Type>();
   const Arg2Type arg2 = ZenUnit::Random<Arg2Type>();
   //
   _voidTwoArgTryCatchCaller.TryCatchCallNonConstMemberFunction(&_classInstance, &Class::NonConstMemberFunction, arg1, arg2, &Class::ExceptionHandler);
   //
   const vector<pair<Arg1Type, Arg2Type>> expectedArguments =
   {
      { arg1, arg2 }
   };
   VECTORS_ARE_EQUAL(expectedArguments, _classInstance.functionCallArguments);

   const ExceptionType expectedException(_classInstance.exceptionMessage.c_str());
   const string expectedExceptionClassNameAndMessage = Type::GetExceptionClassNameAndMessage(&expectedException);
   const vector<string> expectedExceptionHandlerArguments = { expectedExceptionClassNameAndMessage };
   VECTORS_ARE_EQUAL(expectedExceptionHandlerArguments, _classInstance.exceptionHandlerArguments);
}

RUN_TEMPLATE_TESTS(VoidTwoArgTryCatchCallerTests, invalid_argument, string, string)
THEN_RUN_TEMPLATE_TESTS(VoidTwoArgTryCatchCallerTests, runtime_error, fs::path, int)
