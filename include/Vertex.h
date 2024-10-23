#ifndef VERTEX_H
#define VERTEX_H

struct Vertex {
    float x, y, z;    // Position
    float r, g, b;    // Color
    float u, v;       // Texture coordinates
    float ambientOcclusion; 
};

#endif // VERTEX_H