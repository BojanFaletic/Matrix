# Matrix
This repo contain simple matrix class. It is used for performing dot product on 2d vectors.
AI is just matrix multiplication :).

For my CPU (i7-7820) additional performance can be achieved with.

``` shell
clang++ -O2 -march=haswell
```
Performance increase is cca 2% compared without specifying architecture.

Matrix has support for sparse dot product. If input vector has some of values zero, then
multiplication in that row is not required. For initial testing if sparsity is 60% percent then
performance increase 57% compared with normal dot product.

## Building
``` shell
    make test_fixed && ./test_fixed
```