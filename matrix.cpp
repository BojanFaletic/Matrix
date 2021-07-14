#include "matrix.hpp"
#include <iostream>
#include <sstream>

float matrix::el(int y, int x) const {
  int i = (is_transposed) ? x * n + y : y * m + x;
  return mat[i];
}

matrix::matrix() {
  n = 0;
  m = 0;
  is_transposed = false;
  mat = nullptr;
}

matrix::matrix(std::vector<std::vector<float>> const &in_mat) {
  n = in_mat.size();
  m = in_mat[0].size();
  is_transposed = false;

  mat = new float[n * m];
  uint32_t it = 0;
  for (std::vector<float> const &v : in_mat) {
    for (float e : v) {
      mat[it++] = e;
    }
  }
}

matrix matrix::T() {
  matrix M = *this;
  M.is_transposed ^= 1;
  M.n = m;
  M.m = n;
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
  out.n = n;
  out.m = m;
  out.mat = new float[n * m];
  uint32_t it = 0;

  for (uint32_t i = 0; i < a.n; i++) {
    for (uint32_t j = 0; j < b.m; j++) {
      float accum = 0;
      for (uint32_t k = 0; k < a.m; k++) {
        accum += a.el(i, k) * b.el(k, j);
      }
      out.mat[it++] = accum;
    }
  }
  return out;
}

matrix matrix::zeros(int y, int x) {
  matrix M;
  M.mat = new float[y * x];

  M.n = y;
  M.m = x;
  for (int i = 0; i < x * y; i++) {
    M.mat[i] = 0;
  }
  return M;
}

matrix matrix::ones(int y, int x) {
  matrix M;
  M.mat = new float[y * x];

  M.n = y;
  M.m = x;
  for (int i = 0; i < x * y; i++) {
    M.mat[i] = 1;
  }
  return M;
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
  std::pair<uint32_t, uint32_t> M_size{M.n, M.m};
  std::pair<uint32_t, uint32_t> C_size{C.n, C.m};

  if (M_size != C_size) {
    std::cerr << "Hadamard product unable matrix shape not valid\n";
    exit(1);
  }

  matrix A = M;
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    A.mat[i] *= C.mat[i];
  }
  return A;
}

std::ostream &operator<<(std::ostream &out, matrix const &M) {
  out << "[";
  for (uint32_t i = 0; i < M.n * M.m; i++) {
    if (i % M.m == 0) {
      out << ((i == 0) ? "[ " : " [ ");
    }
    uint32_t y = i / M.m;
    uint32_t x = i % M.m;
    out << M.el(y, x) << " ";

    if (i % M.m == M.m - 1) {
      out << ((i == M.n * M.m - 1) ? "]" : "]\n");
    }
  }
  out << "]\n";
  return out;
}