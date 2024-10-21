#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"
#include "Vertex.h"
#include "Triangle.h"
#include <vector>

class Chunk {
public:
    Chunk();
    Chunk(int xOffset, int zOffset);
    Block chunk[16][256][16];
    std::vector<Triangle> triangles;

    int xOffset, zOffset, seed;
    void generateChunk();
    Block generateBlock(int x, int y, int z);
    void initializeMesh();
};

#endif // CHUNK_H