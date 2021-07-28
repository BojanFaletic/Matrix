#include "matrix3.hpp"
#include "matrix.hpp"
#include "matrix1.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

std::array<uint32_t, 3> matrix3::shape() const {
  return {dim[1], dim[2], dim[3]};
}

void matrix3::size(uint32_t z, uint32_t n, uint32_t m) { dim = {0, z, n, m}; }
uint32_t matrix3::shape(uint32_t const axis) const { return shape()[axis]; }

matrix3::matrix3() { size(0, 0, 0); }

matrix3::matrix3(uint32_t const y, uint32_t const n, uint32_t const m) {
  size(y, n, m);
  mat = new float[size()];
}

matrix3::~matrix3() {
  delete[] mat;
  mat = nullptr;
}

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

void matrix3::operator=(matrix_generic const &m) {
  size(m.shape(1), m.shape(2), m.shape(3));
  delete[] mat;
  mat = new float[m.size()];
  copy(m);
}

matrix3 matrix3::zeros(uint32_t z, uint32_t n, uint32_t m) {
  matrix3 M(z, n, m);
  M.init_zero();
  return M;
}

matrix3 matrix3::ones(uint32_t z, uint32_t n, uint32_t m) {
  matrix3 M(z, n, m);
  M.init_ones();
  return M;
}

matrix3 matrix3::random(uint32_t z, uint32_t n, uint32_t m) {
  matrix3 M(z, n, m);
  M.init_random();
  return M;
}

matrix matrix3::squeeze(uint32_t dim) const {
  if (dim != 0) {
    std::cout << "not implemented\n;";
    exit(1);
  }
  // this is not implement yet
  matrix M(shape(1), shape(2) * shape(3));
  for (uint32_t z = 0; z < this->dim[1]; z++) {
    for (uint32_t n = 0; n < this->dim[2]; n++) {
      for (uint32_t m = 0; z < this->dim[3]; m++) {
        M(z, n * m) = (*this)(z, n, m);
      }
    }
  }
  return M;
};

/******************************************************************************/
/***************************** Operators **************************************/
/******************************************************************************/
float &matrix3::operator()(uint32_t const z, uint32_t const n,
                           uint32_t const m) {
  return mat[idx(z, n, m)];
}

float matrix3::operator()(uint32_t const z, uint32_t const n,
                          uint32_t const m) const {
  return mat[idx(z, n, m)];
}

std::ostream &operator<<(std::ostream &out, matrix3 const &M) {
  out << std::setprecision(2);
  out << "[";
  for (uint32_t z = 0; z < M.shape(0); z++) {
    out << ((z == 0) ? "[" : " [");
    for (uint32_t n = 0; n < M.shape(1); n++) {
      out << ((n == 0) ? "[" : "  [");
      for (uint32_t m = 0; m < M.shape(2); m++) {
        out << M(z, n, m) << " ";
      }
      if (n == M.shape(1) - 1 && z == M.shape(0) - 1) {
        out << "]]]\n";
        return out;
      }
      out << ((n == M.shape(1) - 1) ? "]" : "]\n");
    }
    out << "]\n";
  }
  out << "]\n";
  return out;
}