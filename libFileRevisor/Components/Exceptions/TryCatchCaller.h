#pragma once

template<typename ClassType, typename ArgumentType>
class TryCatchCaller
{
public:
   virtual int TryCatchCall(
      ClassType* nonConstClassPointer,
      int (ClassType::* nonConstMemberFunction)(ArgumentType),
      ArgumentType argument,
      int (ClassType::* constMemberExceptionHandlerFunction)(const exception&, ArgumentType) const) const
   {
      try
      {
         const int exitCode = (nonConstClassPointer->*nonConstMemberFunction)(argument);
         return exitCode;
      }
      catch (const exception& ex)
      {
         const int exceptionHandlerExitCode = (nonConstClassPointer->*constMemberExceptionHandlerFunction)(ex, argument);
         return exceptionHandlerExitCode;
      }
   }

   virtual ~TryCatchCaller() = default;
};
