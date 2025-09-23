#pragma once

namespace Utils
{
   template<typename ClassType, typename T, typename Arg2Type, typename Arg3Type, typename Arg4Type>
   class FourArgMemberFunctionForEacher
   {
   public:
      virtual ~FourArgMemberFunctionForEacher() = default;

      using ConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type, Arg3Type, Arg4Type) const;
      using NonConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type, Arg3Type, Arg4Type);

      virtual void CallConstMemberFunctionWithEachElement(
         const vector<T>& elements,
         const ClassType* constClassPointer,
         ConstMemberFunctionType constMemberFunction,
         Arg2Type arg2,
         Arg3Type arg3,
         Arg4Type arg4) const
      {
         for (const T& element : elements)
         {
            (constClassPointer->*constMemberFunction)(element, arg2, arg3, arg4);
         }
      }

      virtual void CallNonConstMemberFunctionWithEachElement(
         const vector<T>& elements,
         ClassType* nonConstClassPointer,
         NonConstMemberFunctionType nonConstMemberFunction,
         Arg2Type arg2,
         Arg3Type arg3,
         Arg4Type arg4) const
      {
         for (const T& element : elements)
         {
            (nonConstClassPointer->*nonConstMemberFunction)(element, arg2, arg3, arg4);
         }
      }
   };
}
