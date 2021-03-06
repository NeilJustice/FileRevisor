#pragma once
#if defined __linux__|| defined __APPLE__
#include <cxxabi.h>
#endif

class Type
{
   friend class TypeTests;
private:
   static std::unordered_map<const char*, std::string> s_mangledToDemangledTypeName;
public:
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
   static const std::string* TypeInfoToTypeName(const std::type_info& typeInfo)
   {
      const char* const mangledTypeName = typeInfo.name();
      const std::unordered_map<const char*, std::string>::const_iterator findIter =
         s_mangledToDemangledTypeName.find(mangledTypeName);
      if (findIter == s_mangledToDemangledTypeName.end())
      {
         const std::string demangledTypeName = Demangle(mangledTypeName);
         const std::pair<std::unordered_map<const char*, std::string>::const_iterator, bool>
            emplaceResult = s_mangledToDemangledTypeName.emplace(mangledTypeName, demangledTypeName);
         const std::string* const cachedDemangledTypeName = &emplaceResult.first->second;
         return cachedDemangledTypeName;
      }
      const std::string* cachedDemangledTypeName = &findIter->second;
      return cachedDemangledTypeName;
   }

#if defined __linux__|| defined __APPLE__
   static std::string Demangle(const char* mangledTypeName)
   {
      int demangleStatus = -1;
      std::unique_ptr<char, void(*)(void*)> demangledTypeNamePointer(
         abi::__cxa_demangle(mangledTypeName, nullptr, nullptr, &demangleStatus),
         std::free);
      release_assert(demangleStatus == 0);
      std::string demangledTypeName(demangledTypeNamePointer.get());
      return demangledTypeName;
   }
#elif _WIN32
   static std::string Demangle(const char* mangledTypeName)
   {
      static const std::regex classStructPattern("(class |struct )");
      std::string typeNameMinusClassAndStruct = std::regex_replace(mangledTypeName, classStructPattern, "");
      return typeNameMinusClassAndStruct;
   }
#endif
};
