// Chunk.cpp
#include "Chunk.h"
#include "Block.h"
#include "Blocknames.h"
#include "Triangle.h"
#include "World.h"
#include <iostream>

Chunk::Chunk() {
    // seed = 0;
    // xOffset = 0;
    // zOffset = 0;
    // generateChunk();
    // initializeMesh(World& world);
}

Chunk::Chunk(int xOffset, int zOffset, World *world) {
    this->xOffset = xOffset;
    this->zOffset = zOffset;
    this->world = world;
    generateChunk();
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
                        {baseX + 0.0f, baseY + 0.0f, baseZ + 0.0f, 0.3f, 0.3f, 0.3f},
                        {baseX + 1.0f, baseY + 0.0f, baseZ + 0.0f, 0.3f, 0.3f, 0.3f},
                        {baseX + 1.0f, baseY + 1.0f, baseZ + 0.0f, 0.3f, 0.3f, 0.3f},
                        {baseX + 0.0f, baseY + 1.0f, baseZ + 0.0f, 0.4f, 0.4f, 0.4f},
                        {baseX + 0.0f, baseY + 0.0f, baseZ + 1.0f, 0.3f, 0.3f, 0.3f},
                        {baseX + 1.0f, baseY + 0.0f, baseZ + 1.0f, 0.3f, 0.3f, 0.3f},
                        {baseX + 1.0f, baseY + 1.0f, baseZ + 1.0f, 0.3f, 0.3f, 0.3f},
                        {baseX + 0.0f, baseY + 1.0f, baseZ + 1.0f, 0.3f, 0.3f, 0.3f}
                    };

                    // Check if each face is exposed to air, considering neighboring chunks if necessary
                    if (world->getBlock(xOffset + x, y, zOffset + z + 1).type == AIR) {
                        triangles.push_back({vertices[4], vertices[5], vertices[6]});
                        triangles.push_back({vertices[4], vertices[6], vertices[7]});
                    }

                    if (world->getBlock(xOffset + x, y, zOffset + z - 1).type == AIR) {
                        triangles.push_back({vertices[0], vertices[2], vertices[1]});
                        triangles.push_back({vertices[0], vertices[3], vertices[2]});
                    }

                    if (world->getBlock(xOffset + x - 1, y, zOffset + z).type == AIR) {
                        triangles.push_back({vertices[0], vertices[7], vertices[4]});
                        triangles.push_back({vertices[0], vertices[3], vertices[7]});
                    }

                    if (world->getBlock(xOffset + x + 1, y, zOffset + z).type == AIR) {
                        triangles.push_back({vertices[1], vertices[6], vertices[5]});
                        triangles.push_back({vertices[1], vertices[2], vertices[6]});
                    }

                    if (world->getBlock(xOffset + x, y + 1, zOffset + z).type == AIR) {
                        triangles.push_back({vertices[3], vertices[6], vertices[7]});
                        triangles.push_back({vertices[3], vertices[2], vertices[6]});
                    }

                    if (world->getBlock(xOffset + x, y - 1, zOffset + z).type == AIR) {
                        triangles.push_back({vertices[0], vertices[5], vertices[4]});
                        triangles.push_back({vertices[0], vertices[1], vertices[5]});
                    }
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
    if (y < 20) {
        return STONE;
    }
    else if (y < 20 + 13 * sin(x*0.2) * sin(z*0.2)) {
            return STONE;
    }
    return AIR;
}
