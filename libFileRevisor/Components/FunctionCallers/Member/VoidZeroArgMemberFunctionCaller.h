#pragma once

template<typename ClassType>
class VoidZeroArgMemberFunctionCaller
{
public:
   virtual ~VoidZeroArgMemberFunctionCaller() = default;

   virtual void CallConstMemberFunction(const ClassType* classInstance, void (ClassType::* constMemberFunction)() const) const
   {
      (classInstance->*constMemberFunction)();
   }

   virtual void CallNonConstMemberFunction(ClassType* classInstance, void (ClassType::* nonConstMemberFunction)()) const
   {
      (classInstance->*nonConstMemberFunction)();
   }
};
