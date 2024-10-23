#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Block.h"
#include "Triangle.h"
#include "World.h"
#include "PerlinNoise.h"

class World;

class Chunk {
public:
    Chunk(int xOffset, int zOffset, World& world);
    bool isBlockAirOrOutOfBounds(int x, int y, int z);

    void initializeMesh(); 
    void generateChunk();

    Block getBlock(int x, int y, int z) const;
    Block chunk[16][256][16];
    std::vector<Triangle> triangles;
    World& world; 

private:
    int xOffset, zOffset; 

    void addFaceTriangles(glm::vec3* vertices, FaceType face, int neighborX, int neighborY, int neighborZ, Block block);

    Block generateBlock(int x, int y, int z, PerlinNoise& noise);
};

#endif // CHUNK_H
