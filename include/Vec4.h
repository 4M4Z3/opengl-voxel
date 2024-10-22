#ifndef VEC3_H
#define VEC3_H

class Vec4 {
public:
    float w, x, y, z;

    Vec4() : w(0), x(0), y(0), z(0) {}
    Vec4(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
};

#endif // VEC3_H
