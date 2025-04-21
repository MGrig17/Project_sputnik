/**
  * Terminology:
  * "Physical point" -- a point located in 3-dimensional virtual space, which coordinates are stored in km units
  * "Graphical point" -- a point located in 2-dimensional space which corresponds to graphical window. GP coordinates are stored in px units (variables of integer-like type)
  */
#pragma once

#include <windows.h>
#include <vector>

#define pi 3.14159265f

struct GraphPoint {
    POINT point;
    bool mask;
    float r3[3];
};
template<typename T>
struct Triplet {
    T num1;
    T num2;
    T num3;
};
Triplet<float>* ellipse(float* c, float* u, float* v, int N) {
    Triplet<float>* r3 = new Triplet<float>[N];
    for (int i = 0; i < N; i++) {
        float t = 2 * pi * i / (N - 1);
        r3[i].num1 = c[0] + u[0] * std::cos(t) + v[0] * std::sin(t);
        r3[i].num2 = c[1] + u[1] * std::cos(t) + v[1] * std::sin(t);
        r3[i].num3 = c[2] + u[2] * std::cos(t) + v[2] * std::sin(t);
    }
    return r3;
}
