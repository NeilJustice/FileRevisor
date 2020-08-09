#include "pch.h"
#include "libFileRevisor/Components/Exceptions/Exception.h"

TESTS(ExceptionTests)
AFACT(ClassNameAndWhat_ReturnsExceptionClassNameColonWhat)
EVIDENCE

TEST(ClassNameAndWhat_ReturnsExceptionClassNameColonWhat)
{
   const exception e;
#ifdef __linux__
   ARE_EQUAL("std::exception: std::exception", Exception::ClassNameAndWhat(&e));
#elif _WIN32
   ARE_EQUAL("std::exception: Unknown exception", Exception::ClassNameAndWhat(&e));
#endif

   const string what = ZenUnit::Random<string>();
   const runtime_error e2(what);
	ARE_EQUAL("std::runtime_error: " + what, Exception::ClassNameAndWhat(&e2));
}

RUN_TESTS(ExceptionTests)
