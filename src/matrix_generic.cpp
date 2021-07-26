#include "matrix_generic.hpp"
#include "matrix1.hpp"
#include <iostream>

bool matrix_generic::approx_zero(float const f) {
  constexpr float zero_tol = 1e-8;
  return std::abs(f) < zero_tol;
}

uint32_t matrix_generic::size() const {
  uint32_t sum = 0;
  for (uint32_t el : dim) {
    sum *= (el == 0) ? 1 : el;
  }
  return sum;
}

matrix_generic::matrix_generic() {
  mat = nullptr;
  dim = {0, 0, 0, 0};
}

matrix_generic::~matrix_generic() {
  delete mat;
  mat = nullptr;
  dim = {0, 0, 0, 0};
}

std::ostream &operator<<(std::ostream &out, matrix_generic const &M) {
  out << "not implemented\n";
  return out;
}

matrix_generic &matrix_generic::operator=(matrix_generic const &m) {
  if (this == &m)
    return *this;

  delete[] mat;
  this->dim = m.dim;
  mat = new float[size()];
  uint32_t it = 0;

  std::for_each(mat, mat + this->size(), [&](float &f) { f = m.mat[it++]; });
  return *this;
}

matrix_generic operator*(matrix_generic const &M, float n) {
  matrix_generic A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] *= n;
  }
  return A;
}

matrix_generic operator*=(matrix_generic &M, float n) {
  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] *= n;
  }
  return M;
}

matrix_generic operator/=(matrix_generic &M, float n) {
  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] /= n;
  }
  return M;
}

matrix_generic operator/(matrix_generic const &M, float n) {
  matrix_generic A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] /= n;
  }
  return A;
}

matrix_generic operator-(matrix_generic const &M, float n) {
  matrix_generic A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] -= n;
  }
  return A;
}

matrix_generic operator+(matrix_generic const &M, float n) {
  matrix_generic A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] += n;
  }
  return A;
}

matrix_generic operator+=(matrix_generic &M, float n) {
  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] += n;
  }
  return M;
}

matrix_generic operator-=(matrix_generic &M, float n) {
  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] -= n;
  }
  return M;
}

matrix_generic operator*(matrix_generic const &M, matrix_generic const &C) {

  if (M.dim != C.dim) {
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
  if (M.dim != m.dim) {
    std::cerr << "Cannot add matrix_generic of different shape\n";
    exit(1);
  }
  matrix_generic A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] = M.mat[i] + m.mat[i];
  }
  return A;
}
