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
  init_sparsity(*this);
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

void matrix::init_sparsity(matrix &M) {
  std::set<uint32_t> row;
  std::set<uint32_t> column;

  for (uint32_t n_ = 0; n_ < M.shape()[0]; n_++) {
    for (uint32_t m_ = 0; m_ < M.shape()[1]; m_++) {
      if (matrix::approx_zero(M(n_, m_))) {
        row.insert(m_);
        column.insert(n_);
      }
    }
  }
  M.sparsity_cnt_n = row.size() * M.n;
  M.sparsity_cnt_m = column.size() * M.m;
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

matrix matrix::T() const {
  matrix M = *this;
  M.size(m, n);
  M.sparsity_cnt_m = sparsity_cnt_n;
  M.sparsity_cnt_n = sparsity_cnt_m;
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

matrix matrix::sparse_dot_normal(matrix const &a, matrix const &b) {
  matrix out;
  out.size(a.n, b.m);
  out.mat = new float[out.size()];

  for (uint32_t k = 0; k < a.n; k++) {
    float *accum = out.mat + k * b.m;
    for (uint32_t i = 0; i < b.m; i++) {
      accum[i] = 0;
    }
    for (uint32_t j = 0; j < a.m; j++) {
      if (!approx_zero(a(k, j))) {
        for (uint32_t i = 0; i < b.n; i++) {
          accum[i] += a(k, j) * b(j, i);
        }
      }
    }
  }
  return out;
}

matrix matrix::sparse_dot_reverse(matrix const &a, matrix const &b) {
  matrix out;
  out.size(a.n, a.m);
  out.mat = new float[out.size()];

  std::for_each(out.mat, out.mat + out.size(), [](float &f) { f = 0.F; });

  for (uint32_t bn = 0; bn < b.n; bn++) {
    const uint32_t am = bn;
    for (uint32_t bm = 0; bm < b.m; bm++) {
      if (!approx_zero(b(bn, bm))) {
        for (uint32_t an = 0; an < a.n; an++) {
          out(an, bm) += a(an, am) * b(bn, bm);
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
  // find  optimal way of performing dot product
  uint32_t first_option = this->sparsity_cnt_m * b.sparsity_cnt_n;
  uint32_t second_option = this->sparsity_cnt_n * b.sparsity_cnt_m;

  // approximate cost of branch predictor
  uint32_t branch_savings = this->sparsity_cnt_n * 4;
  uint16_t max_savings = std::max(first_option, second_option);

  if (max_savings > branch_savings) {
    if (first_option > second_option) {
      return matrix::sparse_dot_normal(*this, b);
    } else {
      return matrix::sparse_dot_reverse(*this, b);
    }
  }
  return matrix::normal_dot(*this, b);
}

matrix matrix::zeros(uint32_t y, uint32_t x) {
  matrix M;
  M.size(y, x);
  M.mat = new float[M.size()];

  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] = 0;
  }

  M.sparsity_cnt_n = y * x;
  M.sparsity_cnt_m = y * x;
  return M;
}

matrix matrix::ones(uint32_t y, uint32_t x) {
  matrix M;
  M.size(y, x);
  M.mat = new float[M.size()];

  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] = 1;
  }

  M.sparsity_cnt_n = 0;
  M.sparsity_cnt_m = 0;
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

matrix matrix::operator=(matrix const &m) {
  if (this == &m)
    return *this;

  delete[] mat;
  this->n = m.n;
  this->m = m.m;
  mat = new float[size()];
  uint32_t it = 0;

  std::for_each(mat, mat + this->size(), [&](float &f) { f = m.mat[it++]; });
  return *this;
}

bool matrix::operator==(matrix const &m) const {
  if (m.shape() != shape()) {
    return false;
  }
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