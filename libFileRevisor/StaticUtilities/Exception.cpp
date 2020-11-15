#include "pch.h"
#include "libFileRevisor/StaticUtilities/Exception.h"

string Exception::GetExceptionClassNameAndMessage(const exception* ex)
{
   const string exceptionClassNameAndMessage = *Type::GetName(*ex) + ": " + ex->what();
   return exceptionClassNameAndMessage;
}
