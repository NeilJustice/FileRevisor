#pragma once

template<typename ClassType>
class VoidZeroArgMemberFunctionCaller
{
public:
   VoidZeroArgMemberFunctionCaller() {}

   virtual void CallConstMemberFunction(const ClassType* classInstance, void (ClassType::* constMemberFunction)() const) const
   {
      (classInstance->*constMemberFunction)();
   }

   virtual void CallNonConstMemberFunction(ClassType* classInstance, void (ClassType::* nonConstMemberFunction)()) const
   {
      (classInstance->*nonConstMemberFunction)();
   }

   virtual ~VoidZeroArgMemberFunctionCaller() = default;
};
