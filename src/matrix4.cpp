#include "matrix4.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

void matrix4::size(uint32_t y, uint32_t z, uint32_t n, uint32_t m) {
  dim = {y, z, n, m};
}

uint32_t matrix4::shape(uint32_t const axis) const { return dim[axis]; }

matrix4::matrix4() {
  size(0, 0, 0, 0);
  mat = nullptr;
}

matrix4::matrix4(matrix4 const &m) {
  size(m.shape(0), m.shape(1), m.shape(2), m.shape(3));
  mat = new float[size()];
  copy(m);
}

matrix4::matrix4(matrix_generic const &m) {
  size(m.shape(0), m.shape(1), m.shape(2), m.shape(3));
  mat = new float[size()];
  copy(m);
}

matrix4::~matrix4() {
  delete[] mat;
  mat = nullptr;
}

matrix4::matrix4(
    std::vector<std::vector<std::vector<std::vector<float>>>> const &in_mat) {
  uint32_t y = in_mat.size();
  uint32_t z = in_mat[0].size();
  uint32_t n = in_mat[0][0].size();
  uint32_t m = in_mat[0][0][0].size();

  this->size(y, z, n, m);
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

void matrix4::operator=(matrix_generic const &m) {
  this->size(m.shape(0), m.shape(1), m.shape(2), m.shape(3));
  delete[] mat;
  mat = new float[size()];
  copy(m);
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
  for (uint32_t y = 0; y < M.shape(0); y++) {
    out << "[";
    for (uint32_t z = 0; z < M.shape(1); z++) {
      out << ((z == 0) ? "[" : "  [");
      for (uint32_t n = 0; n < M.shape(2); n++) {
        out << ((n == 0) ? "[" : "   [");
        for (uint32_t m = 0; m < M.shape(3); m++) {
          out << M(y, z, n, m) << " ";
        }
        if (z == M.shape(1) - 1 && y == M.shape(0) - 1 && n == M.shape(2) - 1) {
          out << "]]]\n";
          return out;
        }
        out << ((n == M.shape(2) - 1) ? "]" : "]\n");
      }
      out << ((z == M.shape(1) - 1) ? "]" : "]\n");
    }
    out << "]\n";
  }
  out << "]\n";
  return out;
}