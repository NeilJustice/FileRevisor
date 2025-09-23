#pragma once

template<typename ClassType, typename Arg1Type>
class VoidOneArgTryCatchCaller
{
public:
   virtual ~VoidOneArgTryCatchCaller() = default;

   using ConstMemberFunctionType = void (ClassType::*)(Arg1Type) const;
   using ConstMemberExceptionFunctionType = void (ClassType::*)(string_view) const;
   using NonConstMemberFunctionType = void (ClassType::*)(Arg1Type);

   virtual void TryCatchCallConstMemberFunction(
      const ClassType* constClassPointer, ConstMemberFunctionType constMemberFunction,
      Arg1Type arg1,
      ConstMemberExceptionFunctionType exceptionHandlerFunction) const
   {
      try
      {
         (constClassPointer->*constMemberFunction)(arg1);
      }
      catch (const exception& ex)
      {
         const string exceptionClassNameAndMessage = Type::GetExceptionClassNameAndMessage(&ex);
         (constClassPointer->*exceptionHandlerFunction)(exceptionClassNameAndMessage);
      }
   }

   virtual void TryCatchCallNonConstMemberFunction(
      ClassType* nonConstClassPointer, NonConstMemberFunctionType nonConstMemberFunction,
      Arg1Type arg1,
      ConstMemberExceptionFunctionType exceptionHandlerFunction) const
   {
      try
      {
         (nonConstClassPointer->*nonConstMemberFunction)(arg1);
      }
      catch (const exception& ex)
      {
         const string exceptionClassNameAndMessage = Type::GetExceptionClassNameAndMessage(&ex);
         (nonConstClassPointer->*exceptionHandlerFunction)(exceptionClassNameAndMessage);
      }
   }
};
