#include "benchmark.hpp"

using nano_s = std::chrono::nanoseconds;
using time_s = std::chrono::high_resolution_clock;
#define N_TIMES 1024

template <typename T> uint32_t time_diff(T F) {
  uint64_t avg_time = 0;

  volatile int i;
  for (i = 0; i < N_TIMES; i++) {
    auto t1 = time_s::now();
    volatile auto out = F();
    auto t2 = time_s::now();
    auto duration = std::chrono::duration_cast<nano_s>(t2 - t1).count();
    avg_time += duration;
    (void)(out);
  }
  return (uint32_t)(avg_time / N_TIMES);
}

void benchmark(matrix m1) {
  auto F = [&] { return m1.dot(m1.T()); };

  bool use_us = false;
  for (int i = 0; i < 3; i++) {
    uint32_t ticks_ns = time_diff(F);
    use_us |= ticks_ns > 1e4;
    if (use_us) {
      std::cout << ticks_ns / 1e3 << " us\n";
    } else {
      std::cout << ticks_ns << " ns\n";
    }
  }
}
