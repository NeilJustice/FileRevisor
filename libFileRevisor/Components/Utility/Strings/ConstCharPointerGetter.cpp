#include "pch.h"
#include "libFileRevisor/Components/Utility/Strings/ConstCharPointerGetter.h"

const char* ConstCharPointerGetter::GetStringConstCharPointer(const string& str) const
{
   const char* const constCharPointer = str.c_str();
   return constCharPointer;
}
