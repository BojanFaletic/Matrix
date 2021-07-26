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
  uint32_t n = M.dim[2];
  uint32_t m = M.dim[3];

  this->size(n, m);
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
  dim = {0, 0, 0, 0};
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
  uint32_t i = n * dim[3] + m;
  return i;
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
  M.sparsity_cnt_n = row.size();
  M.sparsity_cnt_m = column.size();
}

/******************************************************************************/
/****************************** Public ***************************************/
/******************************************************************************/

void matrix::size(uint32_t n, uint32_t m) { dim = {0, 0, n, m}; }

uint32_t matrix::size() const { return dim[2] * dim[3]; }

std::array<uint32_t, 2> matrix::shape() const { return {dim[2], dim[3]}; }

matrix matrix::T() const {
  matrix M = *this;
  uint32_t m = dim[3];
  uint32_t n = dim[2];
  M.size(m, n);
  M.sparsity_cnt_m = sparsity_cnt_n;
  M.sparsity_cnt_n = sparsity_cnt_m;
  return M;
}

matrix matrix::normal_dot(matrix const &a, matrix const &b) {
  matrix out;
  out.size(a.dim[2], b.dim[3]);
  out.mat = new float[out.size()];

  uint32_t it = 0;
  for (uint32_t i = 0; i < a.dim[2]; i++) {
    for (uint32_t j = 0; j < b.dim[3]; j++) {
      float accum = 0;
      for (uint32_t k = 0; k < a.dim[3]; k++) {
        accum += a(i, k) * b(k, j);
      }
      out.mat[it++] = accum;
    }
  }
  return out;
}

matrix matrix::sparse_dot_normal(matrix const &a, matrix const &b) {
  matrix out;
  out.size(a.dim[2], b.dim[3]);
  out.mat = new float[out.size()];

  for (uint32_t k = 0; k < a.dim[2]; k++) {
    float *accum = out.mat + k * b.dim[3];
    for (uint32_t i = 0; i < b.dim[3]; i++) {
      accum[i] = 0;
    }
    for (uint32_t j = 0; j < a.dim[3]; j++) {
      if (!approx_zero(a(k, j))) {
        for (uint32_t i = 0; i < b.dim[2]; i++) {
          accum[i] += a(k, j) * b(j, i);
        }
      }
    }
  }
  return out;
}

matrix matrix::sparse_dot_reverse(matrix const &a, matrix const &b) {
  matrix out;
  out.size(a.dim[2], a.dim[3]);
  out.mat = new float[out.size()];

  std::for_each(out.mat, out.mat + out.size(), [](float &f) { f = 0.F; });

  for (uint32_t bn = 0; bn < b.dim[2]; bn++) {
    const uint32_t am = bn;
    for (uint32_t bm = 0; bm < b.dim[3]; bm++) {
      if (!approx_zero(b(bn, bm))) {
        for (uint32_t an = 0; an < a.dim[2]; an++) {
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
  uint32_t first_option = sparsity_cnt_m * b.dim[3];
  uint32_t second_option = b.sparsity_cnt_n * dim[2];

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
  for (uint32_t n = 0; n < dim[2]; n++) {
    for (uint32_t m = 0; m < dim[3]; m++) {
      M.mat[idx++] = (*this)(n, m);
    }
  }
  return M;
}

/******************************************************************************/
/***************************** Operators **************************************/
/******************************************************************************/

matrix &matrix::operator=(matrix_generic const &m) {
  dim = m.dim;
  delete[] mat;

  mat = new float[size()];
  uint32_t idx = 0;
  std::for_each(this->mat, this->mat + size(),
                [&](float &f) { f = m.mat[idx++]; });
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
    if (i % M.dim[3] == 0) {
      out << ((i == 0) ? "[ " : " [ ");
    }
    uint32_t y = i / M.dim[3];
    uint32_t x = i % M.dim[3];
    out << M(y, x) << " ";

    if (i % M.dim[3] == M.dim[3] - 1) {
      out << ((i == M.dim[2] * M.dim[3] - 1) ? "]" : "]\n");
    }
  }
  out << "]\n";
  return out;
}