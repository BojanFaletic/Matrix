#pragma once


#include <array>
#include <sstream>
#include <stdint.h>
#include <vector>

class matrix1;

class matrix_generic {
public:
  float *mat;
  std::array<uint32_t, 4> dim;

  static bool approx_zero(float const f);

public:
  matrix_generic();
  ~matrix_generic();

  matrix_generic &operator=(matrix_generic const &m);
  bool operator==(matrix_generic const &m);

  // matrix1 flatten() const;
  uint32_t size() const;

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
  friend matrix_generic operator*(matrix_generic const &M, matrix_generic const &C);
  friend matrix_generic operator+(matrix_generic const &M, matrix_generic const &m);
  friend std::ostream &operator<<(std::ostream &out, matrix_generic const &M);
};

