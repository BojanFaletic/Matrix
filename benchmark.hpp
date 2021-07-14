#pragma once

#include "matrix.hpp"
#include <chrono>
#include <iostream>
#include <stdint.h>

using nano_s = std::chrono::nanoseconds;
using time_s = std::chrono::high_resolution_clock;
#define N_TIMES 1024

template <typename T> uint32_t time_diff(T F);
void benchmark(matrix m1);
