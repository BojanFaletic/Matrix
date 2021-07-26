#include "matrix1.hpp"
#include "matrix.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

void matrix1::size(uint32_t m) { dim = {0, 0, 0, m}; }

uint32_t matrix1::idx(uint32_t m) const { return m; }

uint32_t matrix1::size() const { return dim[3]; }

matrix1::matrix1() {
  dim = {0, 0, 0, 0};
  mat = nullptr;
}

matrix1::matrix1(matrix1 const &m) {
  size(m.dim[3]);
  mat = new float[size()];
  uint32_t it = 0;
  std::for_each(mat, mat + this->size(), [&](float &f) { f = m.mat[it++]; });
}

matrix1::~matrix1() {
  delete[] mat;
  mat = nullptr;
}

matrix1::matrix1(std::vector<float> const &in_mat) {
  this->size(in_mat.size());

  mat = new float[size()];
  uint32_t it = 0;
  for (float const &v : in_mat) {
    mat[it++] = v;
  }
}

void matrix1::operator=(matrix_generic const &m) {
  this->size(m.dim[3]);
  delete[] mat;
  mat = new float[size()];
  uint32_t it = 0;
  std::for_each(mat, mat + this->size(), [&](float &f) { f = m.mat[it++]; });
}

matrix1 matrix1::zeros(uint32_t m) {
  matrix1 M;
  M.size(m);
  M.mat = new float[M.size()];

  std::for_each(M.mat, M.mat + M.size(), [](float &f) { f = 0; });
  return M;
}

matrix1 matrix1::ones(uint32_t m) {
  matrix1 M;
  M.size(m);
  M.mat = new float[M.size()];

  std::for_each(M.mat, M.mat + M.size(), [](float &f) { f = 1; });
  return M;
}

matrix1 matrix1::random(uint32_t m) {
  constexpr uint32_t max_number = 1e8;
  matrix1 M;
  M.size(m);
  M.mat = new float[M.size()];

  std::for_each(M.mat, M.mat + M.size(), [](float &f) {
    f *= (double)(rand() % max_number) / max_number;
    ;
  });

  return M;
}

matrix matrix1::unsqueeze(uint32_t dim) const {
  matrix M;
  if (dim == 0) {
    M.size(1, this->dim[3]);
  } else {
    M.size(this->dim[3], 1);
  }
  M.mat = new float[M.size()];
  uint32_t idx = 0;
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f = mat[idx++]; });
  return M;
}

/******************************************************************************/
/***************************** Operators **************************************/
/******************************************************************************/
float matrix1::operator()(uint32_t const n) const { return mat[idx(n)]; }

std::ostream &operator<<(std::ostream &out, matrix1 const &M) {
  out << std::setprecision(2);
  out << "[ ";
  for (uint32_t m = 0; m < M.dim[3]; m++) {
    out << M(m) << " ";
  }
  out << "]\n";
  return out;
}