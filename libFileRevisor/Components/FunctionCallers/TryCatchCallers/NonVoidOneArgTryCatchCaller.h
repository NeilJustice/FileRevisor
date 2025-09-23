#pragma once

template<typename ReturnType, typename ClassType, typename ArgType>
class NonVoidOneArgTryCatchCaller
{
public:
   virtual ~NonVoidOneArgTryCatchCaller() = default;

   virtual ReturnType TryCatchCallConstMemberFunction(
      const ClassType* constClassPointer,
      ReturnType (ClassType::* constMemberFunction)(ArgType) const,
      ArgType argument,
      ReturnType (ClassType::* exceptionHandlerFunction)(const exception&) const) const
   {
      try
      {
         ReturnType returnValue = (constClassPointer->*constMemberFunction)(argument);
         return returnValue;
      }
      catch (const exception& ex)
      {
         ReturnType returnValue = (constClassPointer->*exceptionHandlerFunction)(ex);
         return returnValue;
      }
   }
};
