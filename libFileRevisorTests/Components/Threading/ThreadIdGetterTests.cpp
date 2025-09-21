#include "pch.h"
#include "libFileRevisor/Components/Threading/ThreadIdGetter.h"

TESTS(ThreadIdGetterTests)
AFACT(GetThreadId_ReturnsThisThreadId_CodeCoverage)
EVIDENCE

ThreadIdGetter _threadIdGetter;

TEST(GetThreadId_ReturnsThisThreadId_CodeCoverage)
{
   _threadIdGetter.GetThreadId();
}

RUN_TESTS(ThreadIdGetterTests)
