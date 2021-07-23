#pragma once

#include <sstream>
#include <stdint.h>
#include <vector>

class matrix1 {
public:
  uint32_t n;
  float *mat;

  void size(uint32_t n);
  uint32_t idx(uint32_t n) const;

public:
  matrix1();
  matrix1(matrix1 const &m);
  ~matrix1();
  matrix1(std::vector<float> const &in_mat);

  static matrix1 zeros(uint32_t n);
  static matrix1 ones(uint32_t n);
  static matrix1 random(uint32_t n);
  // take index of matrix
  float &operator()(uint32_t const n);
  float operator()(uint32_t const n) const;
  uint32_t size() const;
};

std::ostream &operator<<(std::ostream &out, matrix1 const &M);

// scalar operators on matrix
matrix1 operator*(matrix1 const &M, float n);
matrix1 operator/(matrix1 const &M, float n);
matrix1 operator+(matrix1 const &M, float n);
matrix1 operator-(matrix1 const &M, float n);
matrix1 operator*=(matrix1 &M, float n);
matrix1 operator/=(matrix1 &M, float n);
matrix1 operator+=(matrix1 &M, float n);
matrix1 operator-=(matrix1 &M, float n);
// operation on matrix
matrix1 operator*(matrix1 const &M, matrix1 const &C);