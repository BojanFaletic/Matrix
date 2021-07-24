#include "matrix.hpp"
#include <benchmark/benchmark.h>
#include <eigen3/Eigen/Dense>
#include <iostream>

using namespace Eigen;

uint32_t mat_size = 32;

void test(MatrixXf const &a, MatrixXf const &b, MatrixXf c) { c = a * b; }
void test_mat(matrix  &a, matrix const &b, matrix c) { c = a.dot(b); }

static void BM_Eigen(benchmark::State &state) {
  MatrixXf a = MatrixXf::Random(mat_size, mat_size);
  MatrixXf b = MatrixXf::Random(mat_size, mat_size);
  MatrixXf c = MatrixXf::Random(mat_size, mat_size);

  for (auto _ : state) {
    // this code gets timed
    test(a, b, c);
  }
}

static void BM_mat(benchmark::State &state) {
  matrix a = matrix::random(mat_size, mat_size);
  matrix b = matrix::random(mat_size, mat_size);
  matrix c = matrix::random(mat_size, mat_size);

  for (auto _ : state) {
    // this code gets timed
    a = b;
    //test_mat(a, b, c);
  }
}

// Register the function as a benchmark
BENCHMARK(BM_Eigen);
BENCHMARK(BM_mat);
// Run the benchmark
BENCHMARK_MAIN();
