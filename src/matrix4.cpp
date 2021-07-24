#include "matrix4.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

void matrix4::size(uint32_t y, uint32_t z, uint32_t n, uint32_t m) {
  this->n = n;
  this->m = m;
  this->z = z;
  this->y = y;
}

uint32_t matrix4::idx(uint32_t y, uint32_t z, uint32_t n, uint32_t m) const {
  uint32_t layer = n * this->m + m;
  uint32_t i3 = layer * this->z + z;
  return i3 * this->y + y;
}

uint32_t matrix4::size() const { return m * n * z * y; }

matrix4::matrix4() {
  n = 0;
  m = 0;
  z = 0;
  y = 0;
  mat = nullptr;
}

matrix4::matrix4(matrix4 const &m) {
  this->size(m.y, m.z, m.n, m.m);
  mat = new float[size()];
  uint32_t it = 0;
  std::for_each(mat, mat + this->size(), [&](float &f) { f = m.mat[it++]; });
}

matrix4::~matrix4() {
  delete[] mat;
  mat = nullptr;
}

matrix4::matrix4(
    std::vector<std::vector<std::vector<std::vector<float>>>> const &in_mat) {
  this->size(in_mat.size(), in_mat[0].size(), in_mat[0][0].size(),
             in_mat[0][0][0].size());

  mat = new float[size()];
  uint32_t it = 0;
  for (std::vector<std::vector<std::vector<float>>> const &sub_v2 : in_mat) {
    for (std::vector<std::vector<float>> const &sub_v : sub_v2) {
      for (std::vector<float> const &v : sub_v) {
        for (float e : v) {
          mat[it++] = e;
        }
      }
    }
  }
}

void matrix4::operator=(matrix4 const &m) {
  this->size(m.y, m.z, m.m, m.n);
  delete[] mat;
  mat = new float[size()];
  uint32_t it = 0;
  std::for_each(mat, mat + this->size(), [&](float &f) { f = m.mat[it++]; });
}

matrix4 matrix4::zeros(uint32_t y, uint32_t z, uint32_t n, uint32_t m) {
  matrix4 M;
  M.size(y, z, n, m);
  M.mat = new float[M.size()];

  std::for_each(M.mat, M.mat + M.size(), [](float &f) { f = 0; });
  return M;
}

matrix4 matrix4::ones(uint32_t y, uint32_t z, uint32_t n, uint32_t m) {
  matrix4 M;
  M.size(y, z, n, m);
  M.mat = new float[M.size()];

  std::for_each(M.mat, M.mat + M.size(), [](float &f) { f = 1; });
  return M;
}

matrix4 matrix4::random(uint32_t y, uint32_t z, uint32_t n, uint32_t m) {
  constexpr uint32_t max_number = 1e8;
  matrix4 M;
  M.size(y, z, n, m);
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
matrix4 operator*(matrix4 const &M, float n) {
  matrix4 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f *= n; });
  return A;
}

matrix4 operator*=(matrix4 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f *= n; });
  return M;
}

matrix4 operator/=(matrix4 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f /= n; });
  return M;
}

matrix4 operator/(matrix4 const &M, float n) {
  matrix4 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f /= n; });
  return A;
}

matrix4 operator-(matrix4 const &M, float n) {
  matrix4 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f -= n; });
  return A;
}

matrix4 operator+(matrix4 const &M, float n) {
  matrix4 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f += n; });
  return A;
}

matrix4 operator+=(matrix4 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f += n; });
  return M;
}

matrix4 operator-=(matrix4 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f -= n; });
  return M;
}

matrix4 operator*(matrix4 const &M, matrix4 const &C) {
  std::array<uint32_t, 4> sz_A{M.y, M.z, M.n, M.m};
  std::array<uint32_t, 4> sz_B{M.y, M.z, M.n, M.m};

  if (sz_A != sz_B) {
    std::cerr << "Hadamard product unable matrix4 shape not valid\n";
    exit(1);
  }

  matrix4 A = M;
  uint32_t idx = 0;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f *= C.mat[idx++]; });
  return A;
}

float &matrix4::operator()(uint32_t const y, uint32_t const z, uint32_t const n,
                           uint32_t const m) {
  return mat[idx(y, z, n, m)];
}

float matrix4::operator()(uint32_t const y, uint32_t const z, uint32_t const n,
                          uint32_t m) const {
  return mat[idx(y, z, n, m)];
}

std::ostream &operator<<(std::ostream &out, matrix4 const &M) {
  out << std::setprecision(2);
  out << "[";
  for (uint32_t y = 0; y < M.y; y++) {
    out << "[";
    for (uint32_t z = 0; z < M.z; z++) {
      out << ((z == 0) ? "[" : "  [");
      for (uint32_t n = 0; n < M.n; n++) {
        out << ((n == 0) ? "[" : "   [");
        for (uint32_t m = 0; m < M.m; m++) {
          out << M(y, z, n, m) << " ";
        }
        if (z == M.z - 1 && y == M.y - 1 && n == M.n - 1) {
          out << "]]]\n";
          return out;
        }
        out << ((n == M.n - 1) ? "]" : "]\n");
      }
      out << ((z == M.z - 1) ? "]" : "]\n");
    }
    out << "]\n";
  }
  out << "]\n";
  return out;
}