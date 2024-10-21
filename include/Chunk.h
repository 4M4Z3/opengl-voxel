#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"
#include "Vertex.h"
#include "Triangle.h"
#include <vector>

class World;

class Chunk {
public:
    Chunk();
    Chunk(int xOffset, int zOffset, World *world);
    Block chunk[16][256][16];
    std::vector<Triangle> triangles;

    int xOffset, zOffset, seed;
    World* world;
    void generateChunk();
    Block generateBlock(int x, int y, int z);
    void initializeMesh();
};

#endif // CHUNK_H
