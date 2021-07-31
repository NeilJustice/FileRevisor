#include "pch.h"
#include "libFileRevisor/StaticUtilities/Exception.h"

TESTS(ExceptionTests)
AFACT(GetExceptionClassNameAndMessage_ReturnsExceptionClassNameColonSpaceWhatText)
EVIDENCE

TEST(GetExceptionClassNameAndMessage_ReturnsExceptionClassNameColonSpaceWhatText)
{
   const exception ex;
#if defined __linux__|| defined __APPLE__
   ARE_EQUAL("std::exception: std::exception", Exception::GetExceptionClassNameAndMessage(&ex));
#elif _WIN32
   ARE_EQUAL("std::exception: Unknown exception", Exception::GetExceptionClassNameAndMessage(&ex));
#endif
   const string exceptionMessage = ZenUnit::Random<string>();
   const runtime_error ex2(exceptionMessage);
   //
   const string exceptionClassNameAndMessage = Exception::GetExceptionClassNameAndMessage(&ex2);
   const string expectedExceptionClassNameAndMessage = "std::runtime_error: " + exceptionMessage;
   ARE_EQUAL(expectedExceptionClassNameAndMessage, exceptionClassNameAndMessage);
}

RUN_TESTS(ExceptionTests)
