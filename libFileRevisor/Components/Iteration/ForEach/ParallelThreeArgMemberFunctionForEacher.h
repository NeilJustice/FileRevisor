#pragma once

template<typename ClassType, typename T, typename Arg2Type, typename Arg3Type>
class ParallelThreeArgMemberFunctionForEacher
{
public:
   virtual ~ParallelThreeArgMemberFunctionForEacher() = default;

   using ConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type, Arg3Type) const;

   virtual void ParallelCallConstMemberFunctionWithEachElement(
      const vector<T>& elements,
      const ClassType* constClassPointer,
      ConstMemberFunctionType constMemberFunction,
      Arg2Type arg2,
      Arg3Type arg3) const
   {
      const auto boundMemberFunction = std::bind(constMemberFunction, constClassPointer, std::placeholders::_1, arg2, arg3);
#ifdef _LIBCPP_VERSION
      std::for_each(elements.cbegin(), elements.cend(), boundMemberFunction);
#else
      std::for_each(std::execution::par_unseq, elements.cbegin(), elements.cend(), boundMemberFunction);
#endif      
   }
};
