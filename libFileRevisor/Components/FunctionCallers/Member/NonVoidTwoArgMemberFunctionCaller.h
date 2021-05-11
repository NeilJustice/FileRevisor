#pragma once

template<typename ReturnType, typename ClassType, typename Arg1Type, typename Arg2Type>
class NonVoidTwoArgMemberFunctionCaller
{
public:
   virtual ReturnType ConstCall(
      const ClassType* classInstance,
      ReturnType(ClassType::* constMemberFunction)(Arg1Type, Arg2Type) const,
      Arg1Type arg1,
      Arg2Type arg2) const
   {
      ReturnType returnValue = (classInstance->*constMemberFunction)(arg1, arg2);
      return returnValue;
   }

   virtual ReturnType NonConstCall(
      ClassType* classInstance,
      ReturnType(ClassType::* nonConstMemberFunction)(Arg1Type, Arg2Type),
      Arg1Type arg1,
      Arg2Type arg2) const
   {
      ReturnType returnValue = (classInstance->*nonConstMemberFunction)(arg1, arg2);
      return returnValue;
   }

   virtual ~NonVoidTwoArgMemberFunctionCaller() = default;
};
