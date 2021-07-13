#include <errno.h>
#include <iostream>
#include <sstream>
#include <vector>

class matrix {
public:
  uint32_t n, m;
  bool is_transposed;
  std::vector<float> mat;

  float el(int y, int x) const {
    int i = (is_transposed) ? x * n + y : y * m + x;
    return mat[i];
  }

public:
  matrix() {
    n = 0;
    m = 0;
    is_transposed = false;
  }

  matrix(std::vector<std::vector<float>> const &in_mat) {
    n = in_mat.size();
    m = in_mat[0].size();
    is_transposed = false;

    mat.reserve(n * m);
    for (std::vector<float> const &v : in_mat) {
      for (float e : v) {
        mat.push_back(e);
      }
    }
  }


  void T() {
    is_transposed ^= 1;

    // swap y,x axis
    uint32_t tmp = n;
    n = m;
    m = tmp;
  }
};

// for printing
std::ostream &operator<<(std::ostream &out, matrix const &M) {
  out << "[";
  for (uint32_t i = 0; i < M.mat.size(); i++) {
    if (i % M.m == 0) {
      out << ((i == 0) ? "[ " : " [ ");
    }
    uint32_t y = i / M.m;
    uint32_t x = i % M.m;
    out << M.el(y, x) << " ";

    if (i % M.m == M.m - 1) {
      out << ((i == M.mat.size() - 1) ? "]" : "]\n");
    }
  }
  out << "]\n";
  return out;
}

int dot(matrix const &a, matrix const &b, matrix &out) {
  // AxB * CxD --> AxD && B == C
  if (a.m != b.n) {
    return EXIT_FAILURE;
  }

  uint32_t n = a.n;
  uint32_t m = b.m;

  out.n = n;
  out.m = m;
  out.mat.reserve(n * m);

  // dot product
  for (uint32_t i = 0; i < a.n; i++) {
    for (uint32_t j = 0; j < b.m; j++) {
      float accum = 0;
      for (uint32_t k = 0; k < a.m; k++) {
        accum += a.el(i, k) * b.el(k, j);
      }
      out.mat.push_back(accum);
    }
  }
  return EXIT_SUCCESS;
}

int main() {

  // tests
  std::vector<std::vector<float>> m{{1, 2, 3}, {4, 5, 6}};
  matrix mm(m);

  matrix mm1(m);
  matrix mm2(m);
  matrix mm3;

  mm2.T();

  std::cout << dot(mm1, mm2, mm3) << "\n";

  std::cout << mm3 << '\n';

  std::cout << "Hello world\n";
  return 0;
}