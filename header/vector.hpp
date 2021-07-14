#pragma once

#include <stdint.h>
#include <vector>
#include <sstream>
#include <iostream>

class vector{
    public:
    float *vect;
    uint32_t m;

    vector();
    vector(std::vector<float> v);
    ~vector();

    static vector zeros(uint32_t n);
    static vector ones(uint32_t n);

    float &operator[](uint32_t const idx);
    float operator[] (uint32_t const idx) const;
};

std::ostream &operator<<(std::ostream &buf, vector const &v);