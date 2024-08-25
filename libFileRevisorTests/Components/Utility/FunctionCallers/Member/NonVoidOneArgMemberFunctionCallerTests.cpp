#include "pch.h"
#include "libFileRevisor/Components/Utility/FunctionCallers/Member/NonVoidOneArgMemberFunctionCaller.h"

template<typename ReturnType, typename ArgType>
TEMPLATE_TESTS(NonVoidOneArgMemberFunctionCallerTests, ReturnType, ArgType)
AFACT(CallConstMemberFunction_CallsNonVoidMemberFunctionOnce_ReturnsReturnValue)
EVIDENCE

class Widget
{
public:
   mutable vector<ArgType> args;
   mutable ReturnType returnValue;

   ReturnType NonVoidFunction(ArgType arg) const
   {
      args.push_back(arg);
      returnValue = ZenUnit::Random<ReturnType>();
      return returnValue;
   }
};

TEST(CallConstMemberFunction_CallsNonVoidMemberFunctionOnce_ReturnsReturnValue)
{
   NonVoidOneArgMemberFunctionCaller<ReturnType, Widget, ArgType> nonVoidOneArgMemberFunctionCaller;
   const Widget widget{};
   const ArgType arg = ZenUnit::Random<ArgType>();
   //
   const ReturnType returnValue = nonVoidOneArgMemberFunctionCaller.CallConstMemberFunction(&widget, &Widget::NonVoidFunction, arg);
   //
   VECTORS_ARE_EQUAL(vector<ArgType>{arg}, widget.args);
   ARE_EQUAL(widget.returnValue, returnValue);
}

RUN_TEMPLATE_TESTS(NonVoidOneArgMemberFunctionCallerTests, char, int)
THEN_RUN_TEMPLATE_TESTS(NonVoidOneArgMemberFunctionCallerTests, int, double)
