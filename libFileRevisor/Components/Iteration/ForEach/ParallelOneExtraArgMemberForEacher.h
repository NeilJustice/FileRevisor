#pragma once
#if defined __GNUG__ && !defined __clang__
#include <parallel/algorithm>
#include <parallel/settings.h>
#endif

template<typename T, typename ClassType, typename MemberFunctionType, typename ExtraArgType>
class ParallelOneExtraArgMemberForEacher
{
public:
   virtual ~ParallelOneExtraArgMemberForEacher() = default;

   virtual void ParallelOneExtraArgMemberForEach(
      const vector<T>& elements,
      const ClassType* constClassPointer,
      MemberFunctionType constMemberFunction,
      ExtraArgType extraArg) const
   {
      const auto extraArgBoundMemberFunction = std::bind(constMemberFunction, constClassPointer, std::placeholders::_1, extraArg);
#if defined __GNUG__ && !defined __clang__
      __gnu_parallel::_Settings settings;
      settings.algorithm_strategy = __gnu_parallel::force_parallel;
      __gnu_parallel::_Settings::set(settings);
      __gnu_parallel::for_each(elements.cbegin(), elements.cend(), extraArgBoundMemberFunction);
#elif defined _WIN32
      std::for_each(std::execution::par_unseq, elements.cbegin(), elements.cend(), extraArgBoundMemberFunction);
#else
      std::for_each(elements.cbegin(), elements.cend(), extraArgBoundMemberFunction);
#endif
   }
};
