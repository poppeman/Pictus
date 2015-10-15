#include <UnitTest++/UnitTest++.h>
#include "orz/stopwatch.h"
#include <chrono>
#include <thread>


TEST(StopWatch_Measure)
{
	Util::StopWatch s;
	s.Start();
	std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(20));
	int time=s.Stop();
	CHECK(abs(time-20)<8);	// Should be enough margin
}

TEST(StopWatch_Pause)
{
	Util::StopWatch s;
	s.Start();
	int time=s.Stop();
	std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(20));
	CHECK(time==s.Stop());	// Should be enough margin
	
}
