#pragma once

#include <array>
#include <sstream>
#include <stdint.h>
#include <variant>
#include <vector>

struct matrix1;

class matrix_generic {
private:
protected:
  float *mat;
  std::array<uint32_t, 4> dim;
  std::array<uint32_t, 4> sparsity;

  uint32_t idx(uint32_t y, uint32_t z, uint32_t n, uint32_t m) const;
  uint32_t idx(uint32_t z, uint32_t n, uint32_t m) const;
  uint32_t idx(uint32_t n, uint32_t m) const;
  uint32_t idx(uint32_t m) const;

  static bool approx_zero(float const f);
  void init_zero();
  void init_ones();
  void init_random();

public:
  matrix_generic();
  matrix_generic(matrix_generic const &M);
  ~matrix_generic();
  void calculate_sparcity();

  matrix_generic &operator=(matrix_generic const &m);
  bool operator==(matrix_generic const &m);

  // matrix1 flatten() const;
  uint32_t size() const;
  std::array<uint32_t, 4> shape() const;
  uint32_t shape(uint32_t axis) const;

  // scalar operators on matrix
  friend matrix_generic operator*(matrix_generic const &M, float n);
  friend matrix_generic operator/(matrix_generic const &M, float n);
  friend matrix_generic operator+(matrix_generic const &M, float n);
  friend matrix_generic operator-(matrix_generic const &M, float n);
  friend matrix_generic operator*=(matrix_generic &M, float n);
  friend matrix_generic operator/=(matrix_generic &M, float n);
  friend matrix_generic operator+=(matrix_generic &M, float n);
  friend matrix_generic operator-=(matrix_generic &M, float n);
  friend bool operator==(matrix_generic const &M, matrix_generic const &A);

  // operation on matrix_generic
  friend matrix_generic operator*(matrix_generic const &M,
                                  matrix_generic const &C);
  friend matrix_generic operator+(matrix_generic const &M,
                                  matrix_generic const &m);

  float &operator()(uint32_t const y, uint32_t const z, uint32_t const n,
                    uint32_t const m);
  float operator()(uint32_t const y, uint32_t const z, uint32_t const n,
                   uint32_t const m) const;

  // iterators
  float *begin();
  float *end();

  void copy(matrix_generic const &m);
};
