#pragma once

template<
   typename ClassType,
   typename Arg1Type,
   typename Arg2Type,
   typename Arg3Type>
class VoidThreeArgMemberFunctionCaller
{
public:
   virtual ~VoidThreeArgMemberFunctionCaller() = default;

   using ConstMemberFunctionType = void (ClassType::*)(Arg1Type, Arg2Type, Arg3Type) const;
   using NonConstMemberFunctionType = void (ClassType::*)(Arg1Type, Arg2Type, Arg3Type);

   virtual void CallConstMemberFunction(
      const ClassType* constClassInstance,
      ConstMemberFunctionType constMemberFunction,
      Arg1Type arg1,
      Arg2Type arg2,
      Arg3Type arg3) const
   {
      (constClassInstance->*constMemberFunction)(arg1, arg2, arg3);
   }

   virtual void CallNonConstMemberFunction(
      ClassType* nonConstClassInstance,
      NonConstMemberFunctionType nonConstMemberFunction,
      Arg1Type arg1,
      Arg2Type arg2,
      Arg3Type arg3) const
   {
      (nonConstClassInstance->*nonConstMemberFunction)(arg1, arg2, arg3);
   }
};
