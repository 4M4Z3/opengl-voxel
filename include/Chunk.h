#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include "Block.h"
#include "Vertex.h"
#include "Triangle.h"
#include "World.h"
#include "PerlinNoise.h"

class World; // Forward declaration

class Chunk {
public:
    Chunk();
    Chunk(int xOffset, int zOffset, World& world);

    Block chunk[16][256][16];
    std::vector<Triangle> triangles;

    int xOffset, zOffset;
    World& world;

    void generateChunk();
    Block generateBlock(int x, int y, int z, PerlinNoise& noise);
    void initializeMesh();
};

#endif // CHUNK_H
