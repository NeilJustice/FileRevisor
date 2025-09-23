#pragma once

template<typename ClassType, typename T>
class OneArgMemberFunctionForEacher
{
public:
   virtual ~OneArgMemberFunctionForEacher() = default;

   using ConstMemberFunctionType = void(ClassType::*)(const T&) const;

   virtual void CallConstMemberFunctionWithEachElement(
      const vector<T>& elements,
      const ClassType* constClassPointer,
      ConstMemberFunctionType constMemberFunction) const
   {
      const typename vector<T>::const_iterator elementsEnd = elements.cend();
      for (typename vector<T>::const_iterator iter = elements.cbegin(); iter != elementsEnd; ++iter)
      {
         const T& element = *iter;
         (constClassPointer->*constMemberFunction)(element);
      }
   }
};
