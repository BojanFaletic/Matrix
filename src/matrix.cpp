#include "matrix.hpp"
#include <array>
#include <iostream>
#include <random>
#include <sstream>

uint32_t matrix::idx(uint32_t n, uint32_t m) const {
  uint32_t i = n * this->m + m;
  return i;
}

void matrix::size(uint32_t n, uint32_t m) {
  this->n = n;
  this->m = m;
}

uint32_t matrix::size() const { return m * n; }

matrix::matrix() {
  n = 0;
  m = 0;
  mat = nullptr;
}

matrix::matrix(float *data, uint32_t n, uint32_t m) {
  mat = data;
  this->size(n, m);
}

matrix::matrix(matrix const &m) {
  this->size(m.n, m.m);
  mat = new float[size()];
  for (uint32_t i = 0; i < m.size(); i++) {
    this->mat[i] = m.mat[i];
  }
}

matrix::~matrix() { delete[] mat; }

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

matrix matrix::T() {
  matrix M(*this);
  M.size(m, n);
  return M;
}

matrix matrix::dot(matrix const &b) {
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

matrix matrix::dot_sparse(matrix const &b) {
  matrix const &a = *this;
  constexpr float approx_zero = 1e-8;
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
      if (std::abs(a(k, j)) > approx_zero) {
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

/******************************************************************************/
/***************************** Operators **************************************/
/******************************************************************************/
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