#include "matrix3.hpp"
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

void matrix3::size(uint32_t y, uint32_t x, uint32_t z) {
  this->n = y;
  this->m = x;
  this->z = z;
}

uint32_t matrix3::size() const { return m * n * z; }

matrix3::matrix3() {
  n = 0;
  m = 0;
  z = 0;
  mat = nullptr;
}

matrix3::matrix3(matrix3 const &m) {
  this->size(m.n, m.m, m.z);
  mat = new float[size()];
  for (uint32_t i = 0; i < m.size(); i++) {
    this->mat[i] = m.mat[i];
  }
}

matrix3::~matrix3() { delete[] mat; }

matrix3::matrix3(std::vector<std::vector<std::vector<float>>> const &in_mat) {
  this->size(in_mat.size(), in_mat[0].size(), in_mat[0].size());

  mat = new float[size()];
  uint32_t it = 0;
  for (std::vector<std::vector<float>> const &sub_v : in_mat) {
    for (std::vector<float> const &v : sub_v) {
      for (float e : v) {
        mat[it++] = e;
      }
    }
  }
}

matrix3 matrix3::zeros(uint32_t y, uint32_t x, uint32_t z) {
  matrix3 M;
  M.size(y, x, z);
  M.mat = new float[M.size()];

  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] = 0;
  }
  return M;
}

matrix3 matrix3::ones(uint32_t y, uint32_t x, uint32_t z) {
  matrix3 M;
  M.size(y, x, z);
  M.mat = new float[M.size()];

  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] = 1;
  }
  return M;
}

matrix3 matrix3::random(uint32_t y, uint32_t x, uint32_t z) {
  constexpr uint32_t max_number = 1e8;
  matrix3 M;
  M.size(y, x, z);
  M.mat = new float[M.size()];

  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] = (double)(rand() % max_number) / max_number;
  }
  return M;
}

/******************************************************************************/
/***************************** Operators **************************************/
/******************************************************************************/
matrix3 operator*(matrix3 const &M, float n) {
  matrix3 A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] *= n;
  }
  return A;
}

matrix3 operator*=(matrix3 &M, float n) {
  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] *= n;
  }
  return M;
}

matrix3 operator/=(matrix3 &M, float n) {
  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] /= n;
  }
  return M;
}

matrix3 operator/(matrix3 const &M, float n) {
  matrix3 A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] /= n;
  }
  return A;
}

matrix3 operator-(matrix3 const &M, float n) {
  matrix3 A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] -= n;
  }
  return A;
}

matrix3 operator+(matrix3 const &M, float n) {
  matrix3 A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] += n;
  }
  return A;
}

matrix3 operator+=(matrix3 &M, float n) {
  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] += n;
  }
  return M;
}

matrix3 operator-=(matrix3 &M, float n) {
  for (uint32_t i = 0; i < M.size(); i++) {
    M.mat[i] -= n;
  }
  return M;
}

matrix3 operator*(matrix3 const &M, matrix3 const &C) {
  uint32_t sz_A[3] = {M.n, M.m, M.z};
  uint32_t sz_B[3] = {M.n, M.m, M.z};

  for (uint32_t i = 0; i < 3; i++) {
    if (sz_A[i] != sz_B[i]) {
      std::cerr << "Hadamard product unable matrix3 shape not valid\n";
      exit(1);
    }
  }

  matrix3 A = M;
  for (uint32_t i = 0; i < M.size(); i++) {
    A.mat[i] *= C.mat[i];
  }
  return A;
}

float &matrix3::operator()(uint32_t const n, uint32_t const m,
                           uint32_t const z) {
  uint32_t layer = n * this->m + m;
  uint32_t i = layer * z;
  return mat[i];
}

float matrix3::operator()(uint32_t const n, uint32_t const m,
                          uint32_t const z) const {
  uint32_t layer = n * this->m + m;
  uint32_t i = layer * z;
  return mat[i];
}

std::ostream &operator<<(std::ostream &out, matrix3 const &M) {
  out << std::setprecision(2);
  out << "[";
  for (uint32_t m = 0; m < M.m; m++) {
    out << ((m == 0) ? "[" : " [");
    for (uint32_t n = 0; n < M.n; n++) {
      out << ((n == 0) ? "[" : "  [");
      for (uint32_t z = 0; z < M.z; z++) {
        out << M(m,n,z) << " ";
      }
      if (n == M.n-1 && m == M.m-1){
        out << "]]]\n";
        return out;
      }
      out << ((n == M.n-1) ? "]" : "]\n");
    }
    out << "]\n";
  }
  out << "]\n";
  return out;
}