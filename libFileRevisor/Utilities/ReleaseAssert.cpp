#include "pch.h"
#include "libFileRevisor/Components/Strings/StringUtil.h"
#include "libFileRevisor/Utilities/CompilerHints.h"
#include "libFileRevisor/Utilities/NOINLINEMacro.h"
#include "libFileRevisor/Utilities/ReleaseAssert.h"

NOINLINE void ThrowLogicError(
   const char* predicateExpressionText,
   const char* filePath,
   long lineNumber,
   const char* functionName)
{
   const string what = String::Concat("release_assert(", predicateExpressionText, ") failed in ", functionName, "()\n", filePath, "(", lineNumber, ")");
   throw logic_error(what);
}

void ReleaseAssert(
   bool predicateExpressionResult,
   const char* predicateExpressionText,
   const char* filePath,
   long lineNumber,
   const char* functionName)
{
#ifdef __linux__
   if (UNLIKELY(!predicateExpressionResult))
#elif _WIN32
   if (!predicateExpressionResult)
#endif
   {
      ThrowLogicError(predicateExpressionText, filePath, lineNumber, functionName);
   }
}
