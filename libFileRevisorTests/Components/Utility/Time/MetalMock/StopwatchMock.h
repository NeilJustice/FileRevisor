#pragma once
#include "libFileRevisor/Components/Utility/Time/Stopwatch.h"

class StopwatchMock : public Metal::Mock<Stopwatch>
{
public:
   METALMOCK_VOID0(Start)
   METALMOCK_NONVOID0(string, StopAndGetElapsedSeconds)
};
