#include "matrix_generic.hpp"
#include "matrix1.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <set>

matrix_generic::matrix_generic(matrix_generic const &M) {
  dim = {M.shape(0), M.shape(1), M.shape(2), M.shape(3)};
  mat = new float[size()];
  copy(M);
}

bool matrix_generic::approx_zero(float const f) {
  constexpr float zero_tol = 1e-8;
  return std::abs(f) < zero_tol;
}

uint32_t matrix_generic::shape(uint32_t axis) const { return shape()[axis]; }

float &matrix_generic::operator()(uint32_t const y, uint32_t const z,
                                  uint32_t const n, uint32_t const m) {
  return mat[idx(y, z, n, m)];
}

float matrix_generic::operator()(uint32_t const y, uint32_t const z,
                                 uint32_t const n, uint32_t const m) const {
  return mat[idx(y, z, n, m)];
}

void matrix_generic::init_zero() { std::fill(begin(), end(), 0); }

void matrix_generic::init_ones() { std::fill(begin(), end(), 1); }
void matrix_generic::init_random() {
  std::for_each(begin(), end(), [](float &f) { f = rand(); });
}

void matrix_generic::calculate_sparcity() {
  std::set<uint32_t> y_s, z_s, n_s, m_s;
  for (uint32_t y = 0; y < shape(0); y++) {
    for (uint32_t z = 0; z < shape(1); z++) {
      for (uint32_t n = 0; n < shape(2); n++) {
        for (uint32_t m = 0; m < shape(3); m++) {
          if (approx_zero((*this)(y, z, n, m))) {
            y_s.insert(y);
            z_s.insert(z);
            n_s.insert(n);
            m_s.insert(m);
          }
        }
      }
    }
  }
  sparsity = {(uint32_t)y_s.size(), (uint32_t)z_s.size(), (uint32_t)n_s.size(),
              (uint32_t)m_s.size()};
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
  std::for_each(begin(), end(), [&](float &f) { f = m.mat[idx++]; });
  sparsity = m.sparsity;
}

std::array<uint32_t, 4> matrix_generic::shape() const { return dim; }

float *matrix_generic::begin() { return &mat[0]; }

float *matrix_generic::end() { return mat + size(); }

matrix_generic::matrix_generic() {
  mat = nullptr;
  sparsity = {0, 0, 0, 0};
  dim = {0, 0, 0, 0};
}

matrix_generic::~matrix_generic() {
  delete[] mat;
  mat = nullptr;
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

bool operator==(matrix_generic const &M, matrix_generic const &A) {
  if (M.shape() != A.shape()) {
    return false;
  }
  for (uint32_t idx = 0; idx < A.size(); idx++) {
    if (M.mat[idx] != A.mat[idx]) {
      return false;
    }
  }
  return true;
}