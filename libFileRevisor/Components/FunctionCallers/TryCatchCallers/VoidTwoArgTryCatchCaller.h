#pragma once

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgTryCatchCaller
{
public:
   virtual ~VoidTwoArgTryCatchCaller() = default;

   virtual void TryCatchCallConstMemberFunction(
      const ClassType* constClassPointer,
      void (ClassType::* constMemberFunction)(Arg1Type, Arg2Type) const,
      Arg1Type arg1, Arg2Type arg2,
      void (ClassType::* exceptionHandlerFunction)(string_view) const) const
   {
      try
      {
         (constClassPointer->*constMemberFunction)(arg1, arg2);
      }
      catch (const exception& ex)
      {
         const string exceptionClassNameAndMessage = Type::GetExceptionClassNameAndMessage(&ex);
         (constClassPointer->*exceptionHandlerFunction)(exceptionClassNameAndMessage);
      }
   }

   virtual void TryCatchCallNonConstMemberFunction(
      ClassType* nonConstClassPointer,
      void (ClassType::* nonConstMemberFunction)(Arg1Type, Arg2Type),
      Arg1Type arg1, Arg2Type arg2,
      void (ClassType::* exceptionHandlerFunction)(string_view) const) const
   {
      try
      {
         (nonConstClassPointer->*nonConstMemberFunction)(arg1, arg2);
      }
      catch (const exception& ex)
      {
         const string exceptionClassNameAndMessage = Type::GetExceptionClassNameAndMessage(&ex);
         (nonConstClassPointer->*exceptionHandlerFunction)(exceptionClassNameAndMessage);
      }
   }
};
