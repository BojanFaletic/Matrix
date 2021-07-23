#include "matrix1.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

void matrix1::size(uint32_t m) { this->m = m; }

uint32_t matrix1::idx(uint32_t m) const { return m; }

uint32_t matrix1::size() const { return m; };

matrix1::matrix1() {
  m = 0;
  mat = nullptr;
}

matrix1::matrix1(matrix1 const &m) {
  this->size(m.m);
  mat = new float[size()];
  uint32_t it = 0;
  std::for_each(mat, mat + this->size(), [&](float &f) { f = m.mat[it++]; });
}

matrix1::~matrix1() { delete[] mat; }

matrix1::matrix1(std::vector<float> const &in_mat) {
  this->size(in_mat.size());

  mat = new float[size()];
  uint32_t it = 0;
  for (float const &v : in_mat) {
    mat[it++] = v;
  }
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
    M.size(1, this->m);
  } else {
    M.size(this->m, 1);
  }
  M.mat = new float[M.size()];
  uint32_t idx = 0;
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f = mat[idx++]; });
  return M;
}

/******************************************************************************/
/***************************** Operators **************************************/
/******************************************************************************/
matrix1 operator*(matrix1 const &M, float n) {
  matrix1 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f *= n; });
  return A;
}

matrix1 operator*=(matrix1 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f *= n; });
  return M;
}

matrix1 operator/=(matrix1 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f /= n; });
  return M;
}

matrix1 operator/(matrix1 const &M, float n) {
  matrix1 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f /= n; });
  return A;
}

matrix1 operator-(matrix1 const &M, float n) {
  matrix1 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f -= n; });
  return A;
}

matrix1 operator+(matrix1 const &M, float n) {
  matrix1 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f += n; });
  return A;
}

matrix1 operator+=(matrix1 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f += n; });
  return M;
}

matrix1 operator-=(matrix1 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f -= n; });
  return M;
}

matrix1 operator*(matrix1 const &M, matrix1 const &C) {
  std::array<uint32_t, 1> sz_A{M.m};
  std::array<uint32_t, 1> sz_B{M.m};

  if (sz_A != sz_B) {
    std::cerr << "Hadamard product unable matrix1 shape not valid\n";
    exit(1);
  }

  matrix1 A = M;
  uint32_t idx = 0;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f *= C.mat[idx++]; });
  return A;
}

float &matrix1::operator()(uint32_t const n) { return mat[idx(n)]; }

float matrix1::operator()(uint32_t const n) const { return mat[idx(n)]; }

std::ostream &operator<<(std::ostream &out, matrix1 const &M) {
  out << std::setprecision(2);
  out << "[";
  for (uint32_t m = 0; m < M.m; m++) {
    out << M(m) << " ";
  }
  out << "]\n";
  return out;
}