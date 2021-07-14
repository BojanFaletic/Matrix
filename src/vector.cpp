#include "vector.hpp"
#include <vector>

vector::vector(){
    m = 0;
    vect = nullptr;
}

vector::vector(std::vector<float> v) {
  uint32_t len = v.size();
  m = len;
  vect = new float[len];
  for (uint32_t i = 0; i < v.size(); i++) {
    vect[i] = v[i];
  }
}

vector vector::zeros(uint32_t n){
    vector V;
    V.vect = new float[n];
    V.m = n;
    for (uint32_t i=0; i<n; i++){
        V.vect[i] = 0;
    }
    return V;
}

vector vector::ones(uint32_t n){
    vector V;
    V.vect = new float[n];
    V.m = n;
    for (uint32_t i=0; i<n; i++){
        V.vect[i] = 1;
    }
    return V;
}


float &vector::operator[](uint32_t const idx){
    if (idx >= m){
        std::cerr << "Idx out of range\n";
        exit(1);
    }
    return vect[idx];
}

float vector::operator[](uint32_t const idx) const{
    if (idx >= m){
        std::cerr << "Idx out of range\n";
        exit(1);
    }
    return vect[idx];
}

std::ostream &operator<<(std::ostream &buf, vector const &v){
    buf << "[ ";
    for (uint32_t i=0; i<v.m; i++){
        buf << v[i] << " ";
    }
    buf << "]";
    return buf;
}

vector::~vector(){
    delete[] vect;
}