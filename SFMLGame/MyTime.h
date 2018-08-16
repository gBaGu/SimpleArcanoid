#pragma once
#include <chrono>

//duration in seconds
using duration_t = std::chrono::duration<int, std::ratio<1>>;
using default_clock = std::chrono::system_clock;
using time_point_t = std::chrono::time_point<default_clock>;