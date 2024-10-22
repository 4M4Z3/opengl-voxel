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

    void initializeMesh(); // Generates the mesh for this chunk
    void generateChunk(); // Generates the blocks in this chunk

    // Get the blocks at a specific position
    Block getBlock(int x, int y, int z) const;
    Block chunk[16][256][16];
    std::vector<Triangle> triangles;
    World& world; // Reference to the world

private:
    int xOffset, zOffset; // Offset of this chunk

    // 3D array of blocks

    // List of triangles representing the mesh

    // Adds triangles for a face of a block
    void addFaceTriangles(glm::vec3* vertices, FaceType face, int neighborX, int neighborY, int neighborZ);

    // Generate a block based on noise
    Block generateBlock(int x, int y, int z, PerlinNoise& noise);
};

#endif // CHUNK_H
