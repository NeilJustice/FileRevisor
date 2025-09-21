#pragma once

template<typename ReturnType, typename ClassType, typename ArgumentType>
class NonVoidOneArgTryCatchCaller
{
public:
   virtual ReturnType TryCatchCallConstMemberFunction(
      const ClassType* constClassPointer,
      ReturnType (ClassType::* constMemberFunction)(ArgumentType) const,
      ArgumentType argument,
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

   virtual ~NonVoidOneArgTryCatchCaller() = default;
};
