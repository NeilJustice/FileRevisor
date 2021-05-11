#include "pch.h"
#include "libFileRevisor/Compiler/CompilerHints.h"

NOINLINE void ThrowLogicError(
   const char* predicateExpressionText,
   const char* filePath,
   long lineNumber,
   const char* functionName)
{
   const string exceptioMessage = String::ConcatValues(
      "release_assert(", predicateExpressionText, ") failed in ", functionName, "()\n", filePath, "(", lineNumber, ")");
   throw logic_error(exceptioMessage);
}

void ReleaseAssert(
   bool predicateExpressionResult,
   const char* predicateExpressionText,
   const char* filePath,
   long lineNumber,
   const char* functionName)
{
#if defined __linux__|| defined __APPLE__
   if (UNLIKELY(!predicateExpressionResult))
#elif _WIN32
   if (!predicateExpressionResult)
#endif
   {
      ThrowLogicError(predicateExpressionText, filePath, lineNumber, functionName);
   }
}
