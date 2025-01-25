#pragma once
#if defined __linux__
#include <cxxabi.h>
#endif

class Type
{
   friend class TypeTests;
private:
   static std::unordered_map<const char*, std::string> s_mangledToDemangledTypeName;
public:
   static string GetExceptionClassNameAndMessage(const exception* ex);

   template<typename T>
   static const std::string* GetName(const T& variable)
   {
      return TypeInfoToTypeName(typeid(variable));
   }

   template<typename T>
   static const std::string* GetName()
   {
      return TypeInfoToTypeName(typeid(T));
   }

   Type() = delete;
private:
   static const std::string* TypeInfoToTypeName(const std::type_info& typeInfo);
#if defined __linux__
   static std::string Demangle(const char* mangledTypeName);
#elif _WIN32
   static std::string Demangle(const char* mangledTypeName);
#endif
};
