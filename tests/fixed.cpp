#include "matrix_fixed.hpp"
#include "benchmark/benchmark.h"

static void BM_basic(benchmark::State &state) {
  for (auto _ : state) {
    // this code gets timed
    benchmark::DoNotOptimize(_);
  }
}

BENCHMARK(BM_basic);
BENCHMARK_MAIN();