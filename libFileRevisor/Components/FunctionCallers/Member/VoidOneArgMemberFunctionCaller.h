#pragma once

template<typename ClassType, typename Arg1Type>
class VoidOneArgMemberFunctionCaller
{
public:
   virtual void CallConstMemberFunction(
      const ClassType* classInstance,
      void (ClassType::* constMemberFunction)(Arg1Type) const,
      Arg1Type arg1) const
   {
      (classInstance->*constMemberFunction)(arg1);
   }

   virtual void CallNonConstMemberFunction(
      ClassType* classInstance,
      void (ClassType::* nonConstMemberFunction)(Arg1Type),
      Arg1Type arg1) const
   {
      (classInstance->*nonConstMemberFunction)(arg1);
   }

   virtual ~VoidOneArgMemberFunctionCaller() = default;
};
