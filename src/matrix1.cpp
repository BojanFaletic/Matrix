#include "matrix1.hpp"
#include "matrix.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

matrix1::matrix1(uint32_t const m) {
  size(m);
  mat = new float[size()];
}

matrix1::matrix1() {
  size(0);
  mat = nullptr;
}

matrix1::matrix1(matrix1 const &m) {
  dim = m.dim;
  mat = new float[size()];
  this->copy(m);
}

matrix1::~matrix1() {
  delete[] mat;
  mat = nullptr;
}

void matrix1::size(uint32_t m) { dim = {0, 0, 0, m}; }

std::array<uint32_t, 1> matrix1::shape() const { return {dim[3]}; }
uint32_t matrix1::shape(uint32_t const axis) const { return shape()[axis]; }

matrix1::matrix1(std::vector<float> const &in_mat) {
  this->size(in_mat.size());
  mat = new float[size()];

  uint32_t it = 0;
  for (float const &v : in_mat) {
    mat[it++] = v;
  }
}

matrix1 matrix1::zeros(uint32_t m) {
  matrix1 M(m);
  std::fill(M.begin(), M.end(), 0);
  return M;
}

matrix1 matrix1::ones(uint32_t m) {
  matrix1 M(m);
  std::fill(M.begin(), M.end(), 1);
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


/******************************************************************************/
/***************************** Operators **************************************/
/******************************************************************************/
float matrix1::operator()(uint32_t const n) const { return mat[idx(n)]; }

void matrix1::operator=(matrix_generic &&m) { m.move(*this); }

void matrix1::operator=(matrix_generic const &m) {
  if (size() != m.size()) {
    size(m.shape(0));
    delete[] mat;
    mat = new float[size()];
  }
  copy(m);
}

void matrix1::operator=(matrix1 const &m) {
  if (size() != m.size()) {
    size(m.shape(0));
    delete[] mat;
    mat = new float[size()];
  }
  copy(m);
}

std::ostream &operator<<(std::ostream &out, matrix1 const &M) {
  out << std::setprecision(2);
  out << "[ ";
  for (uint32_t m = 0; m < M.shape(0); m++) {
    float el = M(m);
    out << el << " ";
  }
  out << "]";
  return out;
}