#include "matrix.hpp"
#include <benchmark/benchmark.h>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <vector>

using namespace Eigen;

uint32_t mat_size = 256;

void test(MatrixXf const &a, MatrixXf const &b, MatrixXf c) { c = a * b; }
void test_mat(matrix &a, matrix const &b, matrix c) { c = a.dot(b); }

static void BM_Eigen(benchmark::State &state) {
  MatrixXf a = MatrixXf::Random(mat_size, mat_size);
  MatrixXf b = MatrixXf::Random(mat_size, mat_size);
  MatrixXf c;

  for (auto _ : state) {
    // this code gets timed
    test(a, b, c);
  }
}

static void BM_mat(benchmark::State &state) {
  matrix a = matrix::random(mat_size, mat_size);
  matrix b = matrix::random(mat_size, mat_size);
  matrix c;

  for (auto _ : state) {
    // this code gets timed
    test_mat(a, b, c);
  }
}

static void BM_copy_constructor(benchmark::State &state) {
  matrix a = matrix::random(mat_size, mat_size);
  matrix b = matrix::random(mat_size, mat_size);
  matrix c;

  for (auto _ : state) {
    // this code gets timed
    c = a;
  }
}

static void BM_test_dot_mult(benchmark::State &state) {
  matrix a = matrix::random(mat_size, mat_size);
  matrix b = matrix::random(mat_size, mat_size);
  matrix c = matrix::random(mat_size, mat_size);

  for (auto _ : state) {
    // this code gets timed
    for (uint32_t idx = 0; idx < mat_size * mat_size; idx++){
      c.mat[idx] += a.mat[idx] * b.mat[idx];
    }
  }
}



// Register the function as a benchmark
BENCHMARK(BM_Eigen);
BENCHMARK(BM_mat);
BENCHMARK(BM_copy_constructor);
BENCHMARK(BM_test_dot_mult);
// Run the benchmark
BENCHMARK_MAIN();
