#include "matrix3.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

void matrix3::size(uint32_t y, uint32_t x, uint32_t z) {
  this->n = y;
  this->m = x;
  this->z = z;
}

uint32_t matrix3::idx(uint32_t n, uint32_t m, uint32_t z) const {
  uint32_t layer = n * this->m + m;
  uint32_t i = layer * z;
  return i;
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
  uint32_t it = 0;
  std::for_each(mat, mat + this->size(), [&](float &f) { f = m.mat[it++]; });
}

matrix3::~matrix3() { delete[] mat; }

matrix3::matrix3(std::vector<std::vector<std::vector<float>>> const &in_mat) {
  this->size(in_mat.size(), in_mat[0].size(), in_mat[0][0].size());

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

  std::for_each(M.mat, M.mat + M.size(), [](float &f) { f = 0; });
  return M;
}

matrix3 matrix3::ones(uint32_t y, uint32_t x, uint32_t z) {
  matrix3 M;
  M.size(y, x, z);
  M.mat = new float[M.size()];

  std::for_each(M.mat, M.mat + M.size(), [](float &f) { f = 1; });
  return M;
}

matrix3 matrix3::random(uint32_t y, uint32_t x, uint32_t z) {
  constexpr uint32_t max_number = 1e8;
  matrix3 M;
  M.size(y, x, z);
  M.mat = new float[M.size()];

  std::for_each(M.mat, M.mat + M.size(), [](float &f) {
    f *= (double)(rand() % max_number) / max_number;
    ;
  });

  return M;
}

matrix1 matrix3::flatten() const {
  matrix1 M;
  M.size(size());
  M.mat = new float[M.size()];

  uint32_t idx = 0;
  for (uint32_t n = 0; n < this->n; n++) {
    for (uint32_t m = 0; m < this->m; m++) {
      for (uint32_t z = 0; z < this->z; z++) {
        M.mat[idx++] = (*this)(n, m, z);
      }
    }
  }
  return M;
}

matrix matrix3::squeeze(uint32_t dim) const {
  matrix M;
  M.size(n, m * z);

  if (dim != 0) {
    std::cout << "not implemented\n;";
    exit(1);
  }

  M.mat = new float[M.size()];
  for (uint32_t n = 0; n < this->n; n++) {
    for (uint32_t m = 0; m < this->m; m++) {
      for (uint32_t z = 0; z < this->z; z++) {
        M(n, m * z) = (*this)(n, m, z);
      }
    }
  }
  return M;
};

/******************************************************************************/
/***************************** Operators **************************************/
/******************************************************************************/
matrix3 operator*(matrix3 const &M, float n) {
  matrix3 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f *= n; });
  return A;
}

matrix3 operator*=(matrix3 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f *= n; });
  return M;
}

matrix3 operator/=(matrix3 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f /= n; });
  return M;
}

matrix3 operator/(matrix3 const &M, float n) {
  matrix3 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f /= n; });
  return A;
}

matrix3 operator-(matrix3 const &M, float n) {
  matrix3 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f -= n; });
  return A;
}

matrix3 operator+(matrix3 const &M, float n) {
  matrix3 A = M;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f += n; });
  return A;
}

matrix3 operator+=(matrix3 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f += n; });
  return M;
}

matrix3 operator-=(matrix3 &M, float n) {
  std::for_each(M.mat, M.mat + M.size(), [&](float &f) { f -= n; });
  return M;
}

matrix3 operator*(matrix3 const &M, matrix3 const &C) {
  std::array<uint32_t, 3> sz_A{M.n, M.m, M.z};
  std::array<uint32_t, 3> sz_B{M.n, M.m, M.z};

  if (sz_A != sz_B) {
    std::cerr << "Hadamard product unable matrix3 shape not valid\n";
    exit(1);
  }

  matrix3 A = M;
  uint32_t idx = 0;
  std::for_each(A.mat, A.mat + A.size(), [&](float &f) { f *= C.mat[idx++]; });
  return A;
}

float &matrix3::operator()(uint32_t const n, uint32_t const m,
                           uint32_t const z) {
  return mat[idx(n, m, z)];
}

float matrix3::operator()(uint32_t const n, uint32_t const m,
                          uint32_t const z) const {
  return mat[idx(n, m, z)];
}

std::ostream &operator<<(std::ostream &out, matrix3 const &M) {
  out << std::setprecision(2);
  out << "[";
  for (uint32_t m = 0; m < M.m; m++) {
    out << ((m == 0) ? "[" : " [");
    for (uint32_t n = 0; n < M.n; n++) {
      out << ((n == 0) ? "[" : "  [");
      for (uint32_t z = 0; z < M.z; z++) {
        out << M(m, n, z) << " ";
      }
      if (n == M.n - 1 && m == M.m - 1) {
        out << "]]]\n";
        return out;
      }
      out << ((n == M.n - 1) ? "]" : "]\n");
    }
    out << "]\n";
  }
  out << "]\n";
  return out;
}