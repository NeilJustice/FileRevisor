#pragma once
#include "libFileRevisor/Components/Time/Stopwatch.h"

class StopwatchMock : public Zen::Mock<Stopwatch>
{
public:
   ZENMOCK_VOID0(Start)
   ZENMOCK_NONVOID0(string, StopAndGetElapsedSeconds)
};
