#include "matrix.hpp"
#include <iostream>
#include <sstream>

float matrix::el(int y, int x) const {
  int i = y * m + x;
  return mat[i];
}

matrix::matrix() {
  n = 0;
  m = 0;
  mat = nullptr;
}

matrix::matrix(matrix const &m) {
  mat = new float[m.m * m.n];
  for (uint32_t i = 0; i < m.n * m.m; i++) {
    this->mat[i] = m.mat[i];
  }
  this->m = m.m;
  n = m.n;
}

matrix::~matrix() { delete[] mat; }

matrix::matrix(std::vector<std::vector<float>> const &in_mat) {
  n = in_mat.size();
  m = in_mat[0].size();

  mat = new float[n * m];
  uint32_t it = 0;
  for (std::vector<float> const &v : in_mat) {
    for (float e : v) {
      mat[it++] = e;
    }
  }
}

matrix matrix::T() {
  matrix M(*this);
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
  out.n = height;
  out.m = width;
  out.mat = new float[height * width];

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

float &matrix::operator()(uint32_t const n, uint32_t const m) {
  return mat[n * this->m + m];
}

float matrix::operator()(uint32_t const n, uint32_t const m) const {
  return mat[n * this->m + m];
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