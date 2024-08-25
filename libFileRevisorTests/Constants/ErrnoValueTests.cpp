#include "pch.h"

TESTS(ErrnoValueTests)
AFACT(Constants_AreExpectedValues)
EVIDENCE

TEST(Constants_AreExpectedValues)
{
   ARE_EQUAL(13, ErrnoValue::PermissionDenied);
}

RUN_TESTS(ErrnoValueTests)
