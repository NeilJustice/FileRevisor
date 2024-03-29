#pragma once

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller
{
public:
   virtual void CallConstMemberFunction(
      const ClassType* classInstance,
      void (ClassType::* constMemberFunction)(Arg1Type, Arg2Type) const,
      Arg1Type arg1, Arg2Type arg2) const
   {
      (classInstance->*constMemberFunction)(arg1, arg2);
   }

   virtual void CallNonConstMemberFunction(
      ClassType* classInstance,
      void (ClassType::* nonConstMemberFunction)(Arg1Type, Arg2Type),
      Arg1Type arg1, Arg2Type arg2) const
   {
      (classInstance->*nonConstMemberFunction)(arg1, arg2);
   }

   virtual ~VoidTwoArgMemberFunctionCaller() = default;
};
