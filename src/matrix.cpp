#include "matrix.hpp"
#include "matrix1.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <vector>

/******************************************************************************/
/**************************** Constructors ************************************/
/******************************************************************************/

matrix::matrix(float *data, uint32_t n, uint32_t m) {
  mat = data;
  this->size(n, m);
}

matrix::matrix(matrix const &M) {
  this->size(M.n, M.m);
  mat = new float[size()];
  uint32_t idx = 0;

  for (uint32_t n = 0; n < shape()[0]; n++) {
    for (uint32_t m = 0; m < shape()[1]; m++) {
      (*this)(n, m) = M(n, m);
    }
  }
  sparsity_cnt_n = M.sparsity_cnt_n;
  sparsity_cnt_m = M.sparsity_cnt_m;
}

matrix::matrix() {
  n = 0;
  m = 0;
  sparsity_cnt_n = 0;
  sparsity_cnt_m = 0;
  mat = nullptr;
}

matrix::matrix(std::vector<std::vector<float>> const &in_mat) {
  this->size(in_mat.size(), in_mat[0].size());

  mat = new float[size()];
  uint32_t it = 0;
  for (std::vector<float> const &v : in_mat) {
    for (float e : v) {
      mat[it++] = e;
    }
  }
}

matrix::~matrix() {
  delete[] mat;
  mat = nullptr;
}

/******************************************************************************/
/****************************** Private ***************************************/
/******************************************************************************/

uint32_t matrix::idx(uint32_t n, uint32_t m) const {
  uint32_t i = n * this->m + m;
  return i;
}

bool matrix::approx_zero(float const f) {
  constexpr float zero_tol = 1e-8;
  return std::abs(f) < zero_tol;
}

void matrix::init_sparsity() {
  std::set<uint32_t> row;
  std::set<uint32_t> column;

  for (uint32_t n = 0; n < shape()[0]; n++) {
    for (uint32_t m = 0; m < shape()[1]; m++) {
      if (approx_zero((*this)(n, m))) {
        row.insert(m);
        column.insert(n);
      }
    }
  }
  sparsity_cnt_n = row.size() * n;
  sparsity_cnt_m = column.size() * m;
}

/******************************************************************************/
/****************************** Public ***************************************/
/******************************************************************************/

void matrix::size(uint32_t n, uint32_t m) {
  this->n = n;
  this->m = m;
}

uint32_t matrix::size() const { return m * n; }

std::array<uint32_t, 2> matrix::shape() const { return {n, m}; }

void matrix::operator=(matrix const &m) {
  size(m.n, m.m);
  delete[] mat;
  mat = new float[size()];
  uint32_t it = 0;
  std::for_each(mat, mat + this->size(), [&](float &f) { f = m.mat[it++]; });
}

matrix matrix::T() const {
  matrix M = *this;
  M.size(m, n);
  return M;
}

matrix matrix::normal_dot(matrix const &a, matrix const &b) {
  matrix out;
  out.size(a.n, b.m);
  out.mat = new float[out.size()];

  uint32_t it = 0;
  for (uint32_t i = 0; i < a.n; i++) {
    for (uint32_t j = 0; j < b.m; j++) {
      float accum = 0;
      for (uint32_t k = 0; k < a.m; k++) {
        accum += a(i, k) * b(k, j);
      }
      out.mat[it++] = accum;
    }
  }
  return out;
}

matrix matrix::sparse_dot(matrix const &a, matrix const &b) {
  uint32_t width = b.m;
  uint32_t height = a.n;
  uint32_t depth = a.m;

  matrix out;
  out.size(height, width);
  out.mat = new float[out.size()];

  for (uint32_t k = 0; k < height; k++) {
    float *accum = out.mat + k * width;
    for (uint32_t i = 0; i < width; i++) {
      accum[i] = 0;
    }
    for (uint32_t j = 0; j < depth; j++) {
      if (!approx_zero(a(k, j))) {
        // vectorization goes here
        for (uint32_t i = 0; i < width; i++) {
          accum[i] += a(k, j) * b(j, i);
        }
      }
    }
  }

  return out;
}

matrix matrix::dot(matrix const &b) const {
  matrix const &a = *this;
  if (a.m != b.n) {
    std::cerr << "Matrix shape not compatible\n";
    exit(1);
  }

  uint32_t n = a.n;
  uint32_t m = b.m;

  matrix out;
  out.size(n, m);
  out.mat = new float[out.size()];

  uint32_t it = 0;
  for (uint32_t i = 0; i < a.n; i++) {
    for (uint32_t j = 0; j < b.m; j++) {
      float accum = 0;
      for (uint32_t k = 0; k < a.m; k++) {
        accum += a(i, k) * b(k, j);
      }
      out.mat[it++] = accum;
    }
  }
  return out;
}

matrix matrix::dot_sparse(matrix const &b) const {
  matrix const &a = *this;
  if (a.m != b.n) {
    std::cerr << "Matrix shape not compatible\n";
    exit(1);
  }

  uint32_t width = b.m;
  uint32_t height = a.n;
  uint32_t depth = a.m;

  matrix out;
  out.size(height, width);
  out.mat = new float[out.size()];

  for (uint32_t k = 0; k < height; k++) {
    float *accum = out.mat + k * width;
    for (uint32_t i = 0; i < width; i++) {
      accum[i] = 0;
    }
    for (uint32_t j = 0; j < depth; j++) {
      if (!approx_zero(a(k, j))) {
        // vectorization goes here
        for (uint32_t i = 0; i < width; i++) {
          accum[i] += a(k, j) * b(j, i);
        }
      }
    }
  }

  return out;
}

matrix matrix::zeros(uint32_t y, uint32_t x) {
  matrix M;
  M.size(y, x);
  M.mat = new float[M.size()];

  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] = 0;
  }
  return M;
}

matrix matrix::ones(uint32_t y, uint32_t x) {
  matrix M;
  M.size(y, x);
  M.mat = new float[M.size()];

  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] = 1;
  }
  return M;
}

matrix matrix::random(uint32_t y, uint32_t x) {
  constexpr uint32_t max_number = 1e8;
  matrix M;
  M.size(y, x);
  M.mat = new float[M.size()];

  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] = (double)(rand() % max_number) / max_number;
  }
  return M;
}

matrix1 matrix::flatten() const {
  matrix1 M;
  M.size(size());
  M.mat = new float[M.size()];

  uint32_t idx = 0;
  for (uint32_t n = 0; n < this->n; n++) {
    for (uint32_t m = 0; m < this->m; m++) {
      M.mat[idx++] = (*this)(n, m);
    }
  }
  return M;
}

/******************************************************************************/
/***************************** Operators **************************************/
/******************************************************************************/

bool matrix::operator==(matrix const &m) const {
  for (uint32_t idx = 0; idx < size(); idx++) {
    if (m.mat[idx] != mat[idx]) {
      return false;
    }
  }
  return true;
}

matrix operator*(matrix const &M, float n) {
  matrix A = M;
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    A.mat[i] *= n;
  }
  return A;
}

matrix operator*=(matrix &M, float n) {
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    M.mat[i] *= n;
  }
  return M;
}

matrix operator/=(matrix &M, float n) {
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    M.mat[i] /= n;
  }
  return M;
}

matrix operator/(matrix const &M, float n) {
  matrix A = M;
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    A.mat[i] /= n;
  }
  return A;
}

matrix operator-(matrix const &M, float n) {
  matrix A = M;
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    A.mat[i] -= n;
  }
  return A;
}

matrix operator+(matrix const &M, float n) {
  matrix A = M;
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    A.mat[i] += n;
  }
  return A;
}

matrix operator+=(matrix &M, float n) {
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    M.mat[i] += n;
  }
  return M;
}

matrix operator-=(matrix &M, float n) {
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    M.mat[i] -= n;
  }
  return M;
}

matrix operator*(matrix const &M, matrix const &C) {
  std::array<uint32_t, 2> sz_A{M.n, M.m};
  std::array<uint32_t, 2> sz_B{M.n, M.m};

  if (sz_A != sz_B) {
    std::cerr << "Hadamard product unable matrix shape not valid\n";
    exit(1);
  }

  matrix A = M;
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    A.mat[i] *= C.mat[i];
  }
  return A;
}

matrix operator+(matrix const &M, matrix const &m) {
  if (M.n != m.n || M.m != m.m) {
    std::cerr << "Cannot add matrix of different shape\n";
    exit(1);
  }
  matrix A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] = M.mat[i] + m.mat[i];
  }
  return A;
}

float &matrix::operator()(uint32_t const n, uint32_t const m) {
  return mat[idx(n, m)];
}

float matrix::operator()(uint32_t const n, uint32_t const m) const {
  return mat[idx(n, m)];
}

std::ostream &operator<<(std::ostream &out, matrix const &M) {
  out << "[";
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    if (i % M.m == 0) {
      out << ((i == 0) ? "[ " : " [ ");
    }
    uint32_t y = i / M.m;
    uint32_t x = i % M.m;
    out << M(y, x) << " ";

    if (i % M.m == M.m - 1) {
      out << ((i == M.n * M.m - 1) ? "]" : "]\n");
    }
  }
  out << "]\n";
  return out;
}