#pragma once
#include "libFileRevisor/Components/Time/Stopwatch.h"

class StopwatchMock : public Zen::Mock<Stopwatch>
{
public:
   METALMOCK_VOID0(Start)
   METALMOCK_NONVOID0(string, StopAndGetElapsedSeconds)
};
