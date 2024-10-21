#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vertex.h"
struct Triangle {
    Vertex v0, v1, v2;
    float depth;
};


#endif