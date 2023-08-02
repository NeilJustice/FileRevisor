#include "pch.h"
#include "libFileRevisor/Components/Utility/FunctionCallers/TryCatchCallers/NonVoidOneArgTryCatchCaller.h"

template<typename ReturnType, typename ExceptionType, typename ArgumentType>
TEMPLATE_TESTS(NonVoidOneArgTryCatchCallerTests, ReturnType, ExceptionType, ArgumentType)
AFACT(TryCatchCallConstMemberFunction_ReturnsResultFromCallingConstMemberFunction)
AFACT(TryCatchCallConstMemberFunction_ConstMemberFunctionThrowsAnException_CallsExceptionHandler_ReturnsExceptionHandlerReturnValue)
EVIDENCE

class Class
{
public:
   bool doThrowException = false;
   mutable vector<ArgumentType> functionCallArguments;
   ReturnType returnValue;

   string exceptionMessage;
   mutable vector<string> exceptionHandlerFunctionCallArguments;
   ReturnType exceptionHandlerReturnValue;

   Class()
      : returnValue(ZenUnit::Random<ReturnType>())
      , exceptionMessage(ZenUnit::Random<string>())
      , exceptionHandlerReturnValue(ZenUnit::Random<ReturnType>())
   {
   }

   ReturnType ConstMemberFunction(ArgumentType argument) const
   {
      functionCallArguments.emplace_back(argument);
      if (doThrowException)
      {
         throw ExceptionType(exceptionMessage.c_str());
      }
      return returnValue;
   }

   ReturnType ExceptionHandler(const exception& ex) const
   {
      string exceptionClassNameAndMessage = Type::GetExceptionClassNameAndMessage(&ex);
      exceptionHandlerFunctionCallArguments.emplace_back(std::move(exceptionClassNameAndMessage));
      return exceptionHandlerReturnValue;
   }
};

Class _classInstance;
NonVoidOneArgTryCatchCaller<ReturnType, Class, ArgumentType> _nonVoidOneArgTryCatchCaller;

TEST(TryCatchCallConstMemberFunction_ReturnsResultFromCallingConstMemberFunction)
{
   _classInstance.doThrowException = false;
   const ArgumentType argument = ZenUnit::Random<ArgumentType>();
   //
   const ReturnType returnValue = _nonVoidOneArgTryCatchCaller.TryCatchCallConstMemberFunction(
      &_classInstance, &Class::ConstMemberFunction, argument, &Class::ExceptionHandler);
   //
   vector<ArgumentType> expectedArguments = { argument };
   VECTORS_ARE_EQUAL(expectedArguments, _classInstance.functionCallArguments);
   IS_EMPTY(_classInstance.exceptionHandlerFunctionCallArguments);
   ARE_EQUAL(_classInstance.returnValue, returnValue);
}

TEST(TryCatchCallConstMemberFunction_ConstMemberFunctionThrowsAnException_CallsExceptionHandler_ReturnsExceptionHandlerReturnValue)
{
   _classInstance.doThrowException = true;
   const ArgumentType argument = ZenUnit::Random<ArgumentType>();
   //
   const ReturnType returnValue = _nonVoidOneArgTryCatchCaller.TryCatchCallConstMemberFunction(
      &_classInstance, &Class::ConstMemberFunction, argument, &Class::ExceptionHandler);
   //
   vector<ArgumentType> expectedArguments = { argument };
   VECTORS_ARE_EQUAL(expectedArguments, _classInstance.functionCallArguments);

   ExceptionType expectedException(_classInstance.exceptionMessage.c_str());
   const string expectedExceptionClassNameAndMessage = Type::GetExceptionClassNameAndMessage(&expectedException);
   vector<string> expectedExceptionHandlerFunctionCallArguments = { expectedExceptionClassNameAndMessage };
   VECTORS_ARE_EQUAL(expectedExceptionHandlerFunctionCallArguments, _classInstance.exceptionHandlerFunctionCallArguments);

   ARE_EQUAL(_classInstance.exceptionHandlerReturnValue, returnValue);
}

RUN_TEMPLATE_TESTS(NonVoidOneArgTryCatchCallerTests, int, invalid_argument, string)
THEN_RUN_TEMPLATE_TESTS(NonVoidOneArgTryCatchCallerTests, string, runtime_error, fs::path)
