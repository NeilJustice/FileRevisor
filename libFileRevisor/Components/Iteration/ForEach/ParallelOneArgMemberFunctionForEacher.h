#pragma once

template<typename ClassType, typename T>
class ParallelOneArgMemberFunctionForEacher
{
public:
   virtual ~ParallelOneArgMemberFunctionForEacher() = default;

   using ConstMemberFunctionType = void(ClassType::*)(const T&) const;

   virtual void ParallelCallConstMemberFunctionWithEachElement(
      const vector<T>& elements,
      const ClassType* constClassPointer,
      ConstMemberFunctionType constMemberFunction) const
   {
      const auto boundConstMemberFunction = std::bind(constMemberFunction, constClassPointer, std::placeholders::_1);
#ifdef _LIBCPP_VERSION
      std::for_each(elements.cbegin(), elements.cend(), boundConstMemberFunction);
#else
      std::for_each(std::execution::par_unseq, elements.cbegin(), elements.cend(), boundConstMemberFunction);
#endif
   }
};
