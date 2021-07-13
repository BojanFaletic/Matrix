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
}

matrix::matrix(std::vector<std::vector<float>> const &in_mat) {
  n = in_mat.size();
  m = in_mat[0].size();
  is_transposed = false;

  mat.reserve(n * m);
  for (std::vector<float> const &v : in_mat) {
    for (float e : v) {
      mat.push_back(e);
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
  out.mat.reserve(n * m);

  for (uint32_t i = 0; i < a.n; i++) {
    for (uint32_t j = 0; j < b.m; j++) {
      float accum = 0;
      for (uint32_t k = 0; k < a.m; k++) {
        accum += a.el(i, k) * b.el(k, j);
      }
      out.mat.push_back(accum);
    }
  }
  return out;
}

matrix matrix::zeros(int y, int x) {
  matrix M;
  M.mat.clear();
  M.mat.reserve(y * x);

  M.n = y;
  M.m = x;
  for (int i = 0; i < x * y; i++) {
    M.mat.push_back(0);
  }
  return M;
}

matrix matrix::ones(int y, int x) {
  matrix M;
  M.mat.clear();
  M.mat.reserve(y * x);

  M.n = y;
  M.m = x;
  for (int i = 0; i < x * y; i++) {
    M.mat.push_back(1);
  }
  return M;
}

matrix operator*(matrix const &M, float n) {
  matrix A = M;
  for (float &el : A.mat) {
    el *= n;
  }
  return A;
}

matrix operator*=(matrix &M, float n) {
  for (float &el : M.mat) {
    el *= n;
  }
  return M;
}

matrix operator/=(matrix &M, float n) {
  for (float &el : M.mat) {
    el /= n;
  }
  return M;
}

matrix operator/(matrix const &M, float n) {
  matrix A = M;
  for (float &el : A.mat) {
    el /= n;
  }
  return A;
}

matrix operator-(matrix const &M, float n) {
  matrix A = M;
  for (float &el : A.mat) {
    el -= n;
  }
  return A;
}

matrix operator+(matrix const &M, float n) {
  matrix A = M;
  for (float &el : A.mat) {
    el += n;
  }
  return A;
}

matrix operator+=(matrix &M, float n) {
  for (float &el : M.mat) {
    el += n;
  }
  return M;
}

matrix operator-=(matrix &M, float n) {
  for (float &el : M.mat) {
    el -= n;
  }
  return M;
}

std::ostream &operator<<(std::ostream &out, matrix const &M) {
  out << "[";
  for (uint32_t i = 0; i < M.mat.size(); i++) {
    if (i % M.m == 0) {
      out << ((i == 0) ? "[ " : " [ ");
    }
    uint32_t y = i / M.m;
    uint32_t x = i % M.m;
    out << M.el(y, x) << " ";

    if (i % M.m == M.m - 1) {
      out << ((i == M.mat.size() - 1) ? "]" : "]\n");
    }
  }
  out << "]\n";
  return out;
}
