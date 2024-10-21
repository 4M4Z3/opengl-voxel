#include "Chunk.h"
#include "Blocknames.h"
#include "Block.h"
#include "TextureMapping.h"
#include "Triangle.h"
#include "World.h"
#include <iostream>

Chunk::Chunk() {
    // Default constructor
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
                int blockType = chunk[x][y][z].type; // Use the type field directly
                if (blockType != AIR) {
                    float baseX = x + xOffset;
                    float baseY = y;
                    float baseZ = z + zOffset;

                    // Get texture coordinates for this block type
                    BlockTexture blockTexture = getBlockTexture(blockType); 

Vertex vertices[8] = {
                        {baseX + 0.0f, baseY + 0.0f, baseZ + 0.0f, 1.0f, 1.0f, 1.0f, blockTexture.side.uMin, blockTexture.side.vMin}, // 0
                        {baseX + 1.0f, baseY + 0.0f, baseZ + 0.0f, 1.0f, 1.0f, 1.0f, blockTexture.side.uMax, blockTexture.side.vMin}, // 1
                        {baseX + 1.0f, baseY + 1.0f, baseZ + 0.0f, 1.0f, 1.0f, 1.0f, blockTexture.side.uMax, blockTexture.side.vMax}, // 2
                        {baseX + 0.0f, baseY + 1.0f, baseZ + 0.0f, 1.0f, 1.0f, 1.0f, blockTexture.side.uMin, blockTexture.side.vMax}, // 3
                        {baseX + 0.0f, baseY + 0.0f, baseZ + 1.0f, 1.0f, 1.0f, 1.0f, blockTexture.side.uMin, blockTexture.side.vMin}, // 4
                        {baseX + 1.0f, baseY + 0.0f, baseZ + 1.0f, 1.0f, 1.0f, 1.0f, blockTexture.side.uMax, blockTexture.side.vMin}, // 5
                        {baseX + 1.0f, baseY + 1.0f, baseZ + 1.0f, 1.0f, 1.0f, 1.0f, blockTexture.side.uMax, blockTexture.side.vMax}, // 6
                        {baseX + 0.0f, baseY + 1.0f, baseZ + 1.0f, 1.0f, 1.0f, 1.0f, blockTexture.side.uMin, blockTexture.side.vMax}  // 7
                    };

                    // Set up triangles for each face with the appropriate texture
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
        return Block(STONE);
    }
    else if (y < 20 + 7 * sin(x * 0.2) * sin(z * 0.2)
    + 13 * sin(-x * 0.033) + 12 * cos(-z * 0.05)) {
        return Block(STONE);
    }
    return Block(AIR);
}
