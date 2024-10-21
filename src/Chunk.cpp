// Chunk.cpp
#include "Chunk.h"
#include "Block.h"
#include "Blocknames.h"
#include "Triangle.h"
#include <iostream>

Chunk::Chunk() {
    seed = 0;
    xOffset = 0;
    zOffset = 0;
    generateChunk();
    initializeMesh();
}

Chunk::Chunk(int xOffset, int zOffset) {
    this->xOffset = xOffset;
    this->zOffset = zOffset;
    generateChunk();
    initializeMesh();
}

void Chunk::initializeMesh() {
    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 256; ++y) {
            for (int z = 0; z < 16; ++z) {
                if (chunk[x][y][z].type == STONE) {
                    float baseX = x + xOffset;
                    float baseY = y;
                    float baseZ = z + zOffset;

                    Vertex vertices[8] = {
                        {baseX + 0.0f, baseY + 0.0f, baseZ + 0.0f, 0.3f, 0.3f, 0.3f}, // Bottom-left-back (darker grey)
                        {baseX + 1.0f, baseY + 0.0f, baseZ + 0.0f, 0.3f, 0.3f, 0.3f}, // Bottom-right-back (darker grey)
                        {baseX + 1.0f, baseY + 1.0f, baseZ + 0.0f, 0.3f, 0.3f, 0.3f}, // Top-right-back (darker grey)
                        {baseX + 0.0f, baseY + 1.0f, baseZ + 0.0f, 0.4f, 0.4f, 0.4f}, // Top-left-back (darker grey)
                        {baseX + 0.0f, baseY + 0.0f, baseZ + 1.0f, 0.3f, 0.3f, 0.3f}, // Bottom-left-front (darker grey)
                        {baseX + 1.0f, baseY + 0.0f, baseZ + 1.0f, 0.3f, 0.3f, 0.3f}, // Bottom-right-front (darker grey)
                        {baseX + 1.0f, baseY + 1.0f, baseZ + 1.0f, 0.3f, 0.3f, 0.3f}, // Top-right-front (darker grey)
                        {baseX + 0.0f, baseY + 1.0f, baseZ + 1.0f, 0.3f, 0.3f, 0.3f}  // Top-left-front (darker grey)
                    };
std::cout << "test";
                    triangles.push_back({vertices[4], vertices[5], vertices[6]});
                    triangles.push_back({vertices[4], vertices[6], vertices[7]});

                    triangles.push_back({vertices[0], vertices[2], vertices[1]});
                    triangles.push_back({vertices[0], vertices[3], vertices[2]});

                    triangles.push_back({vertices[0], vertices[7], vertices[4]});
                    triangles.push_back({vertices[0], vertices[3], vertices[7]});

                    // Right face (vertices 1, 5, 6, 2)
                    triangles.push_back({vertices[1], vertices[6], vertices[5]});
                    triangles.push_back({vertices[1], vertices[2], vertices[6]});

                    // Top face (vertices 3, 7, 6, 2)
                    triangles.push_back({vertices[3], vertices[6], vertices[7]});
                    triangles.push_back({vertices[3], vertices[2], vertices[6]});

                    // Bottom face (vertices 0, 4, 5, 1)
                    triangles.push_back({vertices[0], vertices[5], vertices[4]});
                    triangles.push_back({vertices[0], vertices[1], vertices[5]});
                }
            }
        }
    }
}


void Chunk::generateChunk() {
    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 256; ++y) {
            for (int z = 0; z < 16; ++z) {
                chunk[x][y][z] = generateBlock(x, y, z);
            }
        }
    }
}

Block Chunk::generateBlock(int x, int y, int z) {
    if (y < 30) {
        return STONE;
    }
    return AIR;
}
