#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <ostream>

template <uint32_t y, uint32_t z, uint32_t n, uint32_t m> class matrixF {
  using mat_t =
      std::array<std::array<std::array<std::array<float, m>, n>, z>, y>;

  mat_t mat;

public:
  static constexpr uint32_t size = y * z * n * m;
  matrixF();
  matrixF(mat_t const &M) { mat = M; }

  static mat_t zeros() {
    mat_t M;
    return M;
  };

  void print() {
    using std::cout;
    cout << "[";
  }
};
