#include <iostream>
#include <vector>

class matrix {
  uint32_t n, m;
  bool is_transposed;
  std::vector<float> mat;

  float idx(int y, int x) {
    int i = (is_transposed) ? x * n + y : y * m + x;
    return mat[i];
  }

public:
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

  void print() {
    std::cout << "[";
    for (uint32_t i = 0; i < mat.size(); i++) {
      if (i % m == 0) {
        std::cout << ((i == 0) ? "[ " : " [ ");
      }
      int y = i / m;
      int x = i % m;
      std::cout << idx(y, x) << " ";

      if (i % m == m - 1) {
        std::cout << ((i == mat.size()-1) ? "]" : "]\n");
      }
    }
    std::cout << "]\n";
  }
};

int main() {

  // tests
  std::vector<std::vector<float>> m{{1, 2, 3}, {4,5, 6}};
  matrix mm(m);

  mm.print();

  std::cout << "Hello world\n";
  return 0;
}