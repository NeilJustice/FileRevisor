#include "pch.h"
#include "libFileRevisor/StaticUtilities/Type.h"

unordered_map<const char*, string> Type::s_mangledToDemangledTypeName;

string Type::GetExceptionClassNameAndMessage(const exception* ex)
{
   const string exceptionTypeName = *Type::GetName(*ex);
   const string exceptionMessage = ex->what();
   string exceptionTypeNameAndMessage = Utils::String::ConcatStrings(exceptionTypeName, ": ", exceptionMessage);
   return exceptionTypeNameAndMessage;
}

const string* Type::TypeInfoToTypeName(const type_info& typeInfo)
{
   const char* const mangledTypeName = typeInfo.name();
   const unordered_map<const char*, string>::const_iterator findIter =
      s_mangledToDemangledTypeName.find(mangledTypeName);
   if (findIter == s_mangledToDemangledTypeName.end())
   {
      const string demangledTypeName = Demangle(mangledTypeName);
      const pair<unordered_map<const char*, string>::const_iterator, bool>
         emplaceResult = s_mangledToDemangledTypeName.emplace(mangledTypeName, demangledTypeName);
      const string* const cachedDemangledTypeName = &emplaceResult.first->second;
      return cachedDemangledTypeName;
   }
   const string* cachedDemangledTypeName = &findIter->second;
   return cachedDemangledTypeName;
}

#if defined __linux__
   string Type::Demangle(const char* mangledTypeName)
   {
      int demangleStatus = -1;
      unique_ptr<char, void(*)(void*)> demangledTypeNamePointer(
         abi::__cxa_demangle(mangledTypeName, nullptr, nullptr, &demangleStatus),
         free);
      release_assert(demangleStatus == 0);
      string demangledTypeName(demangledTypeNamePointer.get());
      return demangledTypeName;
   }
#elif _WIN32
   string Type::Demangle(const char* mangledTypeName)
   {
      static const regex classStructPattern("(class |struct )");
      string typeNameMinusClassAndStruct = regex_replace(mangledTypeName, classStructPattern, "");
      return typeNameMinusClassAndStruct;
   }
#endif
