#include "pch.h"

[[noreturn]] static void ThrowLogicError(
   const char* predicateExpressionText,
   const char* filePath,
   long lineNumber,
   const char* functionName)
{
   const string exceptioMessage = Utils::String::ConcatValues(
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
   [[unlikely]] if (!predicateExpressionResult)
   {
      ThrowLogicError(predicateExpressionText, filePath, lineNumber, functionName);
   }
}
