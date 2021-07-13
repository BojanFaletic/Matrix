#pragma once

#include <sstream>
#include <stdint.h>
#include <vector>

class matrix {
public:
  uint32_t n, m;
  bool is_transposed;
  std::vector<float> mat;

  float el(int y, int x) const;

public:
  matrix();
  matrix(std::vector<std::vector<float>> const &in_mat);

  matrix T();
  matrix dot(matrix const &b);
};

std::ostream &operator<<(std::ostream &out, matrix const &M);