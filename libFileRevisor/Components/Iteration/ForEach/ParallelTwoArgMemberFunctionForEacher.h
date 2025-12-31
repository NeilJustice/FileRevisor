#pragma once

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
      std::for_each(std::execution::par_unseq, elements.cbegin(), elements.cend(), boundMemberFunction);
   }
};
