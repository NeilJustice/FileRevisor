#include "pch.h"
#include "libFileRevisor/StaticUtilities/ReleaseAssert.h"

TESTS(ReleaseAssertTests)
AFACT(release_assert_IsTrue_DoesNothing)
AFACT(release_assert_IsFalse_FromOperatorParentheses_ThrowsLogicError)
AFACT(release_assert_IsFalse_FromFunction_ThrowsLogicError)
EVIDENCE

TEST(release_assert_IsTrue_DoesNothing)
{
   release_assert(true);
   const bool trueBool = true;
   release_assert(trueBool);
   release_assert(1 == 1);
}

static void FailReleaseAssertFunction()
{
   try
   {
      release_assert(1 == 0);
   }
   catch (const logic_error& ex)
   {
      const char* const exceptionMessage = ex.what();
      IS_TRUE(String::ContainsSubstring(exceptionMessage, "release_assert(1 == 0) failed in FailReleaseAssertFunction()"));
      IS_TRUE(String::CaseInsensitiveContainsSubstring(exceptionMessage, "ReleaseAssertTests.cpp(22)"));
   }
}

TEST(release_assert_IsFalse_FromOperatorParentheses_ThrowsLogicError)
{
   try
   {
      struct X
      {
         void operator()() const
         {
            release_assert(false);
         }
      };
      const X x;
      x();
      FAIL_TEST("release_assert did not throw"); // LCOV_EXCL_LINE
   }
   catch (const logic_error& ex)
   {
      const char* const exceptionMessage = ex.what();
#if defined __linux__
      IS_TRUE(String::ContainsSubstring(exceptionMessage, "release_assert(false) failed in operator()()"));
#elif _WIN32
      IS_TRUE(String::ContainsSubstring(exceptionMessage, "release_assert(false) failed in operator ()()"));
#endif
      IS_TRUE(String::CaseInsensitiveContainsSubstring(exceptionMessage, "ReleaseAssertTests.cpp(40)"));
   }
}

TEST(release_assert_IsFalse_FromFunction_ThrowsLogicError)
{
   FailReleaseAssertFunction();
}

RUN_TESTS(ReleaseAssertTests)
