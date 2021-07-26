#pragma once

#include "matrix.hpp"
#include <chrono>
#include <iostream>
#include <stdint.h>

template <typename T> uint32_t time_diff(T F);
void benchmark(matrix const &m1);
void benchmark(matrix const &m1, matrix const &m2);