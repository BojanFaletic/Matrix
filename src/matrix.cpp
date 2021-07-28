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

matrix::matrix(matrix_generic const &m){
  size(m.shape(2), m.shape(3));
  mat = new float[size()];
  copy(m);
}

matrix::matrix(uint32_t const n, uint32_t const m) {
  size(n, m);
  mat = new float[size()];
  calculate_sparcity();
}

matrix::matrix(float *data, uint32_t n, uint32_t m) {
  mat = data;
  size(n, m);
  calculate_sparcity();
}

matrix::matrix(matrix const &M) {
  this->size(M.shape(0), M.shape(1));
  mat = new float[size()];
  this->copy(M);
  calculate_sparcity();
}

matrix::matrix() {
  size(0, 0);
  mat = nullptr;
  calculate_sparcity();
}

matrix::matrix(std::vector<std::vector<float>> const &in_mat) {
  uint32_t n = in_mat.size();
  uint32_t m = in_mat[0].size();
  size(n, m);
  mat = new float[size()];

  uint32_t it = 0;
  for (std::vector<float> const &v : in_mat) {
    for (float e : v) {
      mat[it++] = e;
    }
  }
  calculate_sparcity();
}

matrix::~matrix() {
  delete[] mat;
  mat = nullptr;
}

void matrix::size(uint32_t n, uint32_t m) { dim = {0, 0, n, m}; }

std::array<uint32_t, 2> matrix::shape() const { return {dim[2], dim[3]}; }

uint32_t matrix::shape(uint32_t const axis) const {
  return shape()[axis]; }

matrix matrix::T() const {
  uint32_t m = shape(1);
  uint32_t n = shape(0);
  matrix M(m, n);
  M.copy(*this);
  M.sparsity = sparsity;
  std::swap(M.sparsity[2], M.sparsity[3]);
  return M;
}

matrix matrix::normal_dot(matrix const &a, matrix const &b) {
  matrix out;
  out.size(a.dim[2], b.dim[3]);
  out.mat = new float[out.size()];

  uint32_t it = 0;
  for (uint32_t i = 0; i < a.shape(0); i++) {
    for (uint32_t j = 0; j < b.shape(1); j++) {
      float accum = 0;
      for (uint32_t k = 0; k < a.shape(1); k++) {
        accum += a(i, k) * b(k, j);
      }
      out.mat[it++] = accum;
    }
  }
  return out;
}

matrix matrix::sparse_dot_normal(matrix const &a, matrix const &b) {
  matrix out;
  out.size(a.shape(0), b.shape(1));
  out.mat = new float[out.size()];

  for (uint32_t k = 0; k < a.shape(0); k++) {
    float *accum = out.mat + k * b.shape(1);
    for (uint32_t i = 0; i < b.shape(1); i++) {
      accum[i] = 0;
    }
    for (uint32_t j = 0; j < a.shape(1); j++) {
      if (!approx_zero(a(k, j))) {
        for (uint32_t i = 0; i < b.shape(0); i++) {
          accum[i] += a(k, j) * b(j, i);
        }
      }
    }
  }
  return out;
}

matrix matrix::sparse_dot_reverse(matrix const &a, matrix const &b) {
  matrix out;
  out.size(a.shape(0), a.shape(1));
  out.mat = new float[out.size()];

  out.init_zero();

  for (uint32_t bn = 0; bn < b.shape(0); bn++) {
    const uint32_t am = bn;
    for (uint32_t bm = 0; bm < b.shape(1); bm++) {
      if (!approx_zero(b(bn, bm))) {
        for (uint32_t an = 0; an < a.shape(0); an++) {
          out(an, bm) += a(an, am) * b(bn, bm);
        }
      }
    }
  }

  return out;
}

matrix matrix::dot(matrix const &b) const {
  matrix const &a = *this;
  if (a.shape() != b.shape()) {
    std::cerr << "Matrix shape not compatible\n";
    exit(1);
  }
  // find  optimal way of performing dot product
  uint32_t first_option = sparsity[3] * b.shape(1);
  uint32_t second_option = b.sparsity[2] * shape(0);

  if (first_option != 0 || second_option != 0) {
    if (first_option > second_option) {
      return matrix::sparse_dot_normal(*this, b);
    } else {
      return matrix::sparse_dot_reverse(*this, b);
    }
  }
  return matrix::normal_dot(*this, b);
}

matrix matrix::zeros(uint32_t y, uint32_t x) {
  matrix M(y, x);
  M.init_zero();
  return M;
}

matrix matrix::ones(uint32_t y, uint32_t x) {
  matrix M(y, x);
  M.init_ones();
  return M;
}

matrix matrix::random(uint32_t y, uint32_t x) {
  matrix M(y, x);
  M.init_random();
  return M;
}

matrix1 matrix::flatten() const {
  matrix1 M(size());
  M.copy(*this);
  return M;
}

/******************************************************************************/
/***************************** Operators **************************************/
/******************************************************************************/

matrix &matrix::operator=(matrix_generic const &m) {
  size(m.shape(2), m.shape(3));
  delete[] mat;

  mat = new float[size()];
  this->copy(m);
  return *this;
}

float &matrix::operator()(uint32_t const n, uint32_t const m) {
  return mat[idx(n, m)];
}

float matrix::operator()(uint32_t const n, uint32_t const m) const {
  return mat[idx(n, m)];
}

std::ostream &operator<<(std::ostream &out, matrix const &M) {
  out << "[";
  for (uint32_t i = 0; i < M.size(); i++) {
    if (i % M.shape(1) == 0) {
      out << ((i == 0) ? "[ " : " [ ");
    }
    uint32_t y = i / M.shape(1);
    uint32_t x = i % M.shape(1);
    out << M(y, x) << " ";

    if (i % M.shape(1) == M.shape(1) - 1) {
      out << ((i == M.shape(0) * M.shape(1) - 1) ? "]" : "]\n");
    }
  }
  out << "]\n";
  return out;
}