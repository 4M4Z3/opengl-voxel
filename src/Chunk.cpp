// Chunk.cpp
#include "Chunk.h"

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
                Block& block = chunk[x][y][z];
                if (block.type != AIR) {
                    MeshType meshType = block.getMeshType();
                    if (meshType == CUBE) {
                        float baseX = x + xOffset;
                        float baseY = y;
                        float baseZ = z + zOffset;

                        // Get texture coordinates for each face
                        TextureCoords frontTex = world->textureMap.getTextureCoords(block.type, FaceType::FRONT);
                        TextureCoords backTex = world->textureMap.getTextureCoords(block.type, FaceType::BACK);
                        TextureCoords leftTex = world->textureMap.getTextureCoords(block.type, FaceType::LEFT);
                        TextureCoords rightTex = world->textureMap.getTextureCoords(block.type, FaceType::RIGHT);
                        TextureCoords topTex = world->textureMap.getTextureCoords(block.type, FaceType::TOP);
                        TextureCoords bottomTex = world->textureMap.getTextureCoords(block.type, FaceType::BOTTOM);

                        // Define the 8 vertices of the cube (positions only)
                        Vertex baseVertices[8] = {
                            {baseX + 0.0f, baseY + 0.0f, baseZ + 0.0f}, // 0
                            {baseX + 1.0f, baseY + 0.0f, baseZ + 0.0f}, // 1
                            {baseX + 1.0f, baseY + 1.0f, baseZ + 0.0f}, // 2
                            {baseX + 0.0f, baseY + 1.0f, baseZ + 0.0f}, // 3
                            {baseX + 0.0f, baseY + 0.0f, baseZ + 1.0f}, // 4
                            {baseX + 1.0f, baseY + 0.0f, baseZ + 1.0f}, // 5
                            {baseX + 1.0f, baseY + 1.0f, baseZ + 1.0f}, // 6
                            {baseX + 0.0f, baseY + 1.0f, baseZ + 1.0f}  // 7
                        };

                        // Front face (z + 1)
                        if (world->getBlock(xOffset + x, y, zOffset + z + 1).type == AIR) {
                            triangles.push_back({
                                {baseVertices[4].x, baseVertices[4].y, baseVertices[4].z, 1.0f, 1.0f, 1.0f, frontTex.minU, frontTex.maxV},
                                {baseVertices[5].x, baseVertices[5].y, baseVertices[5].z, 1.0f, 1.0f, 1.0f, frontTex.maxU, frontTex.maxV},
                                {baseVertices[6].x, baseVertices[6].y, baseVertices[6].z, 1.0f, 1.0f, 1.0f, frontTex.maxU, frontTex.minV}
                            });
                            triangles.push_back({
                                {baseVertices[4].x, baseVertices[4].y, baseVertices[4].z, 1.0f, 1.0f, 1.0f, frontTex.minU, frontTex.maxV},
                                {baseVertices[6].x, baseVertices[6].y, baseVertices[6].z, 1.0f, 1.0f, 1.0f, frontTex.maxU, frontTex.minV},
                                {baseVertices[7].x, baseVertices[7].y, baseVertices[7].z, 1.0f, 1.0f, 1.0f, frontTex.minU, frontTex.minV}
                            });
                        }

                        // Back face (z - 1)
                        if (world->getBlock(xOffset + x, y, zOffset + z - 1).type == AIR) {
                            triangles.push_back({
                                {baseVertices[0].x, baseVertices[0].y, baseVertices[0].z, 1.0f, 1.0f, 1.0f, backTex.minU, backTex.maxV},
                                {baseVertices[2].x, baseVertices[2].y, baseVertices[2].z, 1.0f, 1.0f, 1.0f, backTex.maxU, backTex.minV},
                                {baseVertices[1].x, baseVertices[1].y, baseVertices[1].z, 1.0f, 1.0f, 1.0f, backTex.maxU, backTex.maxV}
                            });
                            triangles.push_back({
                                {baseVertices[0].x, baseVertices[0].y, baseVertices[0].z, 1.0f, 1.0f, 1.0f, backTex.minU, backTex.maxV},
                                {baseVertices[3].x, baseVertices[3].y, baseVertices[3].z, 1.0f, 1.0f, 1.0f, backTex.minU, backTex.minV},
                                {baseVertices[2].x, baseVertices[2].y, baseVertices[2].z, 1.0f, 1.0f, 1.0f, backTex.maxU, backTex.minV}
                            });
                        }

                        // Left face (x - 1)
                        if (world->getBlock(xOffset + x - 1, y, zOffset + z).type == AIR) {
                            triangles.push_back({
                                {baseVertices[0].x, baseVertices[0].y, baseVertices[0].z, 1.0f, 1.0f, 1.0f, leftTex.maxU, leftTex.maxV},
                                {baseVertices[7].x, baseVertices[7].y, baseVertices[7].z, 1.0f, 1.0f, 1.0f, leftTex.minU, leftTex.minV},
                                {baseVertices[4].x, baseVertices[4].y, baseVertices[4].z, 1.0f, 1.0f, 1.0f, leftTex.minU, leftTex.maxV}
                            });
                            triangles.push_back({
                                {baseVertices[0].x, baseVertices[0].y, baseVertices[0].z, 1.0f, 1.0f, 1.0f, leftTex.maxU, leftTex.maxV},
                                {baseVertices[3].x, baseVertices[3].y, baseVertices[3].z, 1.0f, 1.0f, 1.0f, leftTex.maxU, leftTex.minV},
                                {baseVertices[7].x, baseVertices[7].y, baseVertices[7].z, 1.0f, 1.0f, 1.0f, leftTex.minU, leftTex.minV}
                            });
                        }

                        // Right face (x + 1)
                        if (world->getBlock(xOffset + x + 1, y, zOffset + z).type == AIR) {
                            triangles.push_back({
                                {baseVertices[1].x, baseVertices[1].y, baseVertices[1].z, 1.0f, 1.0f, 1.0f, rightTex.minU, rightTex.maxV},
                                {baseVertices[6].x, baseVertices[6].y, baseVertices[6].z, 1.0f, 1.0f, 1.0f, rightTex.maxU, rightTex.minV},
                                {baseVertices[5].x, baseVertices[5].y, baseVertices[5].z, 1.0f, 1.0f, 1.0f, rightTex.maxU, rightTex.maxV}
                            });
                            triangles.push_back({
                                {baseVertices[1].x, baseVertices[1].y, baseVertices[1].z, 1.0f, 1.0f, 1.0f, rightTex.minU, rightTex.maxV},
                                {baseVertices[2].x, baseVertices[2].y, baseVertices[2].z, 1.0f, 1.0f, 1.0f, rightTex.minU, rightTex.minV},
                                {baseVertices[6].x, baseVertices[6].y, baseVertices[6].z, 1.0f, 1.0f, 1.0f, rightTex.maxU, rightTex.minV}
                            });
                        }

                        // Top face (y + 1)
                        if (world->getBlock(xOffset + x, y + 1, zOffset + z).type == AIR) {
                            triangles.push_back({
                                {baseVertices[3].x, baseVertices[3].y, baseVertices[3].z, 1.0f, 1.0f, 1.0f, topTex.minU, topTex.maxV},
                                {baseVertices[6].x, baseVertices[6].y, baseVertices[6].z, 1.0f, 1.0f, 1.0f, topTex.maxU, topTex.minV},
                                {baseVertices[7].x, baseVertices[7].y, baseVertices[7].z, 1.0f, 1.0f, 1.0f, topTex.minU, topTex.minV}
                            });
                            triangles.push_back({
                                {baseVertices[3].x, baseVertices[3].y, baseVertices[3].z, 1.0f, 1.0f, 1.0f, topTex.minU, topTex.maxV},
                                {baseVertices[2].x, baseVertices[2].y, baseVertices[2].z, 1.0f, 1.0f, 1.0f, topTex.maxU, topTex.maxV},
                                {baseVertices[6].x, baseVertices[6].y, baseVertices[6].z, 1.0f, 1.0f, 1.0f, topTex.maxU, topTex.minV}
                            });
                        }

                        // Bottom face (y - 1)
                        if (world->getBlock(xOffset + x, y - 1, zOffset + z).type == AIR) {
                            triangles.push_back({
                                {baseVertices[0].x, baseVertices[0].y, baseVertices[0].z, 1.0f, 1.0f, 1.0f, bottomTex.minU, bottomTex.maxV},
                                {baseVertices[5].x, baseVertices[5].y, baseVertices[5].z, 1.0f, 1.0f, 1.0f, bottomTex.maxU, bottomTex.minV},
                                {baseVertices[4].x, baseVertices[4].y, baseVertices[4].z, 1.0f, 1.0f, 1.0f, bottomTex.minU, bottomTex.minV}
                            });
                            triangles.push_back({
                                {baseVertices[0].x, baseVertices[0].y, baseVertices[0].z, 1.0f, 1.0f, 1.0f, bottomTex.minU, bottomTex.maxV},
                                {baseVertices[1].x, baseVertices[1].y, baseVertices[1].z, 1.0f, 1.0f, 1.0f, bottomTex.maxU, bottomTex.maxV},
                                {baseVertices[5].x, baseVertices[5].y, baseVertices[5].z, 1.0f, 1.0f, 0.5f, bottomTex.maxU, bottomTex.minV}
                            });
                        }
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
    else if (y < 20 + 7 * sin(x*0.2) * sin(z*0.2)
    + 13 * sin(-x*0.033) + 12* cos(-z*0.05)
    ) {
            return GRASS;
    }
    return AIR;
}
