#pragma once

#include <array>
#include <sstream>
#include <stdint.h>
#include <variant>
#include <vector>

struct matrix1;

class matrix_generic {
protected:
  std::array<uint32_t, 4> dim;

  uint32_t idx(uint32_t y, uint32_t z, uint32_t n, uint32_t m) const;
  uint32_t idx(uint32_t z, uint32_t n, uint32_t m) const;
  uint32_t idx(uint32_t n, uint32_t m) const;
  uint32_t idx(uint32_t m) const;

  static bool approx_zero(float const f);

public:
  uint32_t dim_size;
  float *mat;
  matrix_generic();
  ~matrix_generic();

  matrix_generic &operator=(matrix_generic const &m);
  bool operator==(matrix_generic const &m);

  // matrix1 flatten() const;
  uint32_t size() const;
  std::array<uint32_t, 4> shape() const;

  // scalar operators on matrix
  friend matrix_generic operator*(matrix_generic const &M, float n);
  friend matrix_generic operator/(matrix_generic const &M, float n);
  friend matrix_generic operator+(matrix_generic const &M, float n);
  friend matrix_generic operator-(matrix_generic const &M, float n);
  friend matrix_generic operator*=(matrix_generic &M, float n);
  friend matrix_generic operator/=(matrix_generic &M, float n);
  friend matrix_generic operator+=(matrix_generic &M, float n);
  friend matrix_generic operator-=(matrix_generic &M, float n);

  // operation on matrix_generic
  friend matrix_generic operator*(matrix_generic const &M,
                                  matrix_generic const &C);
  friend matrix_generic operator+(matrix_generic const &M,
                                  matrix_generic const &m);

  // iterators
  float *begin();
  float *end();

  void copy(matrix_generic const &m);
};
