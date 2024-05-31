#pragma once

#include <chrono>

// time of app start
inline auto gl_appStartTimePoint = std::chrono::system_clock::now();

// returns ticks since app started
inline int64_t getTicksSinceAppStart()
{
	using namespace std;

	const auto currTicksSinceAppStart =
		chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - gl_appStartTimePoint);
	return currTicksSinceAppStart.count();
}

bool gl_getCancelStatus();
