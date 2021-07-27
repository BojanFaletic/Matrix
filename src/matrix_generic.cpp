#include "matrix_generic.hpp"
#include "matrix1.hpp"
#include <algorithm>
#include <iostream>

bool matrix_generic::approx_zero(float const f) {
  constexpr float zero_tol = 1e-8;
  return std::abs(f) < zero_tol;
}

uint32_t matrix_generic::idx(uint32_t y, uint32_t z, uint32_t n,
                             uint32_t m) const {
  uint32_t row_size = dim[3];
  uint32_t plane_size = row_size * dim[2];
  uint32_t cube_size = plane_size * dim[1];

  uint32_t row = m;
  uint32_t plane = n * row_size + row;
  uint32_t section = z * plane_size + plane;
  uint32_t tensor = y * cube_size + section;
  return tensor;
}

uint32_t matrix_generic::idx(uint32_t z, uint32_t n, uint32_t m) const {
  return idx(0, z, n, m);
}

uint32_t matrix_generic::idx(uint32_t n, uint32_t m) const {
  return idx(0, 0, n, m);
}

uint32_t matrix_generic::idx(uint32_t m) const { return idx(0, 0, 0, m); }

uint32_t matrix_generic::size() const {
  constexpr std::array<uint32_t, 4> zero = {0, 0, 0, 0};
  if (dim == zero) {
    return 0;
  }

  uint32_t sum = 1;
  for (uint32_t el : dim) {
    sum *= (el == 0) ? 1 : el;
  }
  return sum;
}

void matrix_generic::copy(matrix_generic const &m) {
  if (size() != m.size()) {
    std::cout << "Size not match\n";
    exit(1);
  }
  uint32_t idx = 0;
  std::for_each(begin(), end(), [&](float &f) { f = m.mat[idx++];});
  mat[0] = m.mat[0];
  float el = mat[0];
}

std::array<uint32_t, 4> matrix_generic::shape() const { return dim; }

float *matrix_generic::begin() { return &mat[0]; }

float *matrix_generic::end() { return mat + size(); }

matrix_generic::matrix_generic() {
  mat = nullptr;
  dim = {};
}

matrix_generic::~matrix_generic() {
  // delete mat;
  // mat = nullptr;
  // dim = {0};
}

matrix_generic &matrix_generic::operator=(matrix_generic const &m) {
  if (this == &m)
    return *this;

  delete[] mat;
  this->dim = m.dim;
  mat = new float[size()];
  this->copy(m);
  return *this;
}

matrix_generic operator*(matrix_generic const &M, float n) {
  matrix_generic A = M;
  std::for_each(A.begin(), A.end(), [&](float &f) { f *= n; });
  return A;
}

matrix_generic operator*=(matrix_generic &M, float n) {
  std::for_each(M.begin(), M.end(), [&](float &f) { f *= n; });
  return M;
}

matrix_generic operator/=(matrix_generic &M, float n) {
  std::for_each(M.begin(), M.end(), [&](float &f) { f /= n; });
  return M;
}

matrix_generic operator/(matrix_generic const &M, float n) {
  matrix_generic A = M;
  std::for_each(A.begin(), A.end(), [&](float &f) { f *= n; });
  return A;
}

matrix_generic operator-(matrix_generic const &M, float n) {
  matrix_generic A = M;
  std::for_each(A.begin(), A.end(), [&](float &f) { f -= n; });
  return A;
}

matrix_generic operator-=(matrix_generic &M, float n) {
  std::for_each(M.begin(), M.end(), [&](float &f) { f -= n; });
  return M;
}

matrix_generic operator+(matrix_generic const &M, float n) {
  matrix_generic A = M;
  std::for_each(A.begin(), A.end(), [&](float &f) { f += n; });
  return A;
}

matrix_generic operator+=(matrix_generic &M, float n) {
  std::for_each(M.begin(), M.end(), [&](float &f) { f += n; });
  return M;
}

matrix_generic operator*(matrix_generic const &M, matrix_generic const &C) {
  if (M.shape() != C.shape()) {
    std::cerr << "Hadamard product unable matrix_generic shape not valid\n";
    exit(1);
  }

  matrix_generic A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] *= C.mat[i];
  }
  return A;
}

matrix_generic operator+(matrix_generic const &M, matrix_generic const &m) {
  if (M.shape() != m.shape()) {
    std::cerr << "Cannot add matrix_generic of different shape\n";
    exit(1);
  }
  matrix_generic A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] = M.mat[i] + m.mat[i];
  }
  return A;
}
