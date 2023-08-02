#pragma once
#if defined __GNUG__ && !defined __clang__
#include <parallel/algorithm>
#include <parallel/settings.h>
#endif

template<typename ClassType, typename T, typename Arg2Type>
class ParallelTwoArgMemberFunctionForEacher
{
public:
   virtual ~ParallelTwoArgMemberFunctionForEacher() = default;

   using ConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type) const;

   virtual void ParallelCallConstMemberFunctionWithEachElement(
      const vector<T>& elements,
      const ClassType* constClassPointer,
      ConstMemberFunctionType constMemberFunction,
      Arg2Type arg2) const
   {
      const auto boundMemberFunction = std::bind(constMemberFunction, constClassPointer, std::placeholders::_1, arg2);
#if defined __GNUG__ && !defined __clang__
      __gnu_parallel::_Settings settings;
      settings.algorithm_strategy = __gnu_parallel::force_parallel;
      __gnu_parallel::_Settings::set(settings);
      __gnu_parallel::for_each(elements.cbegin(), elements.cend(), boundMemberFunction);
#elif defined _WIN32
      std::for_each(std::execution::par_unseq, elements.cbegin(), elements.cend(), boundMemberFunction);
#else
      std::for_each(elements.cbegin(), elements.cend(), boundMemberFunction);
#endif
   }
};
