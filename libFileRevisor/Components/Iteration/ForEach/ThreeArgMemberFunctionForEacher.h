#pragma once

template<typename ClassType, typename T, typename Arg2Type, typename Arg3Type>
class ThreeArgMemberFunctionForEacher
{
public:
   ThreeArgMemberFunctionForEacher() noexcept {}
   virtual ~ThreeArgMemberFunctionForEacher() = default;

   using ConstMemberFunctionType = void(ClassType::*)(const T&, Arg2Type, Arg3Type) const;

   virtual void CallConstMemberFunctionWithEachElement(
      const vector<T>& elements,
      const ClassType* constClassPointer,
      ConstMemberFunctionType constMemberFunction,
      Arg2Type arg2Type,
      Arg3Type arg3Type) const
   {
      const typename vector<T>::const_iterator elementsEnd = elements.cend();
      for (typename vector<T>::const_iterator iter = elements.cbegin(); iter != elementsEnd; ++iter)
      {
         const T& element = *iter;
         (constClassPointer->*constMemberFunction)(element, arg2Type, arg3Type);
      }
   }
};
