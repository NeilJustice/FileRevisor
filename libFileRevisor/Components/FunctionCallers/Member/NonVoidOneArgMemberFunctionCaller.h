#pragma once

template<typename ReturnType, typename ClassType, typename ArgType>
class NonVoidOneArgMemberFunctionCaller
{
public:
   virtual ~NonVoidOneArgMemberFunctionCaller() = default;

   virtual ReturnType CallConstMemberFunction(const ClassType* classInstance, ReturnType(ClassType::* memberFunction)(ArgType) const, ArgType arg) const
   {
      ReturnType returnValue = (classInstance->*memberFunction)(arg);
      return returnValue;
   }
};
