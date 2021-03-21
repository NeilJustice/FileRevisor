#pragma once

template<typename T, typename ClassType, typename MemberFunctionType, typename ExtraArgType>
class ParallelOneExtraArgMemberForEacher
{
public:
   virtual ~ParallelOneExtraArgMemberForEacher() = default;

   virtual void ParallelOneExtraArgMemberForEach(
      const std::vector<T>& elements,
      const ClassType* constClassPointer,
      MemberFunctionType constMemberFunction,
      ExtraArgType extraArg) const
   {
      const auto extraArgBoundMemberFunction = std::bind(constMemberFunction, constClassPointer, std::placeholders::_1, extraArg);
      std::for_each(std::execution::par_unseq, elements.cbegin(), elements.cend(), extraArgBoundMemberFunction);
   }
};
