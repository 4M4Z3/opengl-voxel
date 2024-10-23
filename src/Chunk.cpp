#include "Chunk.h"
#include <glm/glm.hpp>
#include <iostream>

Chunk::Chunk(int xOffset, int zOffset, World& world)
    : xOffset(xOffset), zOffset(zOffset), world(world) {
    generateChunk();
}

void Chunk::initializeMesh() {
    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 256; ++y) {
            for (int z = 0; z < 16; ++z) {
                Block& block = chunk[x][y][z];
                if (block.type == AIR) {
                    continue; // Skip rendering air blocks
                }

                MeshType meshType = block.getMeshType();
                if (meshType == CUBE) {
                    glm::vec3 basePos(x + xOffset, y, z + zOffset);

                    glm::vec3 vertices[8] = {
                        basePos,
                        basePos + glm::vec3(1, 0, 0),
                        basePos + glm::vec3(1, 1, 0),
                        basePos + glm::vec3(0, 1, 0),
                        basePos + glm::vec3(0, 0, 1),
                        basePos + glm::vec3(1, 0, 1),
                        basePos + glm::vec3(1, 1, 1),
                        basePos + glm::vec3(0, 1, 1)
                    };

                    // Check if each face should be rendered based on neighboring block type
                    if (shouldRenderFace(block.type, x + 1, y, z)) {
                        addFaceTriangles(vertices, FaceType::RIGHT, x + 1, y, z, block);
                    }
                    if (shouldRenderFace(block.type, x - 1, y, z)) {
                        addFaceTriangles(vertices, FaceType::LEFT, x - 1, y, z, block);
                    }
                    if (shouldRenderFace(block.type, x, y + 1, z)) {
                        addFaceTriangles(vertices, FaceType::TOP, x, y + 1, z, block);
                    }
                    if (shouldRenderFace(block.type, x, y - 1, z)) {
                        addFaceTriangles(vertices, FaceType::BOTTOM, x, y - 1, z, block);
                    }
                    if (shouldRenderFace(block.type, x, y, z + 1)) {
                        addFaceTriangles(vertices, FaceType::FRONT, x, y, z + 1, block);
                    }
                    if (shouldRenderFace(block.type, x, y, z - 1)) {
                        addFaceTriangles(vertices, FaceType::BACK, x, y, z - 1, block);
                    }
                }
            }
        }
    }
}


bool Chunk::shouldRenderFace(BlockType currentBlockType, int x, int y, int z) {
    BlockType neighborType = getBlockTypeAt(x, y, z);

    if (currentBlockType == WATER) {
        return neighborType == AIR;
    }

    return (neighborType == AIR || neighborType == WATER);
}

BlockType Chunk::getBlockTypeAt(int x, int y, int z) {
    if (y < 0 || y >= 256) {
        return AIR;
    }

    int localX = x;
    int localZ = z;

    int worldX = x + xOffset;
    int worldZ = z + zOffset;

    if (x < 0 || x >= 16 || z < 0 || z >= 16) {
        int neighborChunkX = (worldX < 0 ? ((worldX + 1) / 16 - 1) * 16 : (worldX / 16) * 16);
        int neighborChunkZ = (worldZ < 0 ? ((worldZ + 1) / 16 - 1) * 16 : (worldZ / 16) * 16);

        localX = ((x % 16) + 16) % 16;
        localZ = ((z % 16) + 16) % 16;

        Chunk* neighborChunk = world.getChunk(neighborChunkX, neighborChunkZ);
        if (!neighborChunk) {
            return AIR;
        }

        return neighborChunk->chunk[localX][y][localZ].type;
    }

    return chunk[x][y][z].type;
}

void Chunk::addFaceTriangles(glm::vec3* vertices, FaceType face, int neighborX, int neighborY, int neighborZ, Block block) {
    int worldX = neighborX + xOffset;
    int worldZ = neighborZ + zOffset;

    int textureIndex = blockTypeInfos[block.type].textures[face];
    TextureCoords texCoords = world.textureMap.getTextureCoords(block.type, face);

    auto toVertex = [texCoords](const glm::vec3& pos, float u, float v, const glm::vec3& normal) {
        return Vertex{pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, u, v};
    };

    // Set the normal for each face direction
    glm::vec3 normal;
    float intensity = 1.0f; // Full brightness for all faces
    switch (face) {
        case FaceType::FRONT:
            normal = glm::vec3(0.0f, 0.0f, 1.0f);
            triangles.push_back(Triangle(
                toVertex(vertices[4], texCoords.minU, texCoords.maxV, normal), // Bottom-left
                toVertex(vertices[5], texCoords.maxU, texCoords.maxV, normal), // Bottom-right
                toVertex(vertices[7], texCoords.minU, texCoords.minV, normal), // Top-left
                0.0f));
            triangles.push_back(Triangle(
                toVertex(vertices[5], texCoords.maxU, texCoords.maxV, normal), // Bottom-right
                toVertex(vertices[6], texCoords.maxU, texCoords.minV, normal), // Top-right
                toVertex(vertices[7], texCoords.minU, texCoords.minV, normal), // Top-left
                0.0f));
            break;
        case FaceType::BACK:
            normal = glm::vec3(0.0f, 0.0f, -1.0f);
            triangles.push_back(Triangle(
                toVertex(vertices[1], texCoords.maxU, texCoords.maxV, normal), // Bottom-right
                toVertex(vertices[0], texCoords.minU, texCoords.maxV, normal), // Bottom-left
                toVertex(vertices[2], texCoords.maxU, texCoords.minV, normal), // Top-right
                0.0f));
            triangles.push_back(Triangle(
                toVertex(vertices[2], texCoords.maxU, texCoords.minV, normal), // Top-right
                toVertex(vertices[0], texCoords.minU, texCoords.maxV, normal), // Bottom-left
                toVertex(vertices[3], texCoords.minU, texCoords.minV, normal), // Top-left
                0.0f));
            break;
        case FaceType::LEFT:
            normal = glm::vec3(-1.0f, 0.0f, 0.0f);
            triangles.push_back(Triangle(
                toVertex(vertices[0], texCoords.minU, texCoords.maxV, normal), // Bottom-left
                toVertex(vertices[7], texCoords.maxU, texCoords.minV, normal), // Top-right
                toVertex(vertices[3], texCoords.minU, texCoords.minV, normal), // Top-left
                0.0f));
            triangles.push_back(Triangle(
                toVertex(vertices[0], texCoords.minU, texCoords.maxV, normal), // Bottom-left
                toVertex(vertices[4], texCoords.maxU, texCoords.maxV, normal), // Bottom-right
                toVertex(vertices[7], texCoords.maxU, texCoords.minV, normal), // Top-right
                0.0f));
            break;
        case FaceType::RIGHT:
            normal = glm::vec3(1.0f, 0.0f, 0.0f);
            triangles.push_back(Triangle(
                toVertex(vertices[1], texCoords.minU, texCoords.maxV, normal), // Bottom-left
                toVertex(vertices[2], texCoords.minU, texCoords.minV, normal), // Top-left
                toVertex(vertices[6], texCoords.maxU, texCoords.minV, normal), // Top-right
                0.0f));
            triangles.push_back(Triangle(
                toVertex(vertices[1], texCoords.minU, texCoords.maxV, normal), // Bottom-left
                toVertex(vertices[6], texCoords.maxU, texCoords.minV, normal), // Top-right
                toVertex(vertices[5], texCoords.maxU, texCoords.maxV, normal), // Bottom-right
                0.0f));
            break;
        case FaceType::TOP:
            normal = glm::vec3(0.0f, 1.0f, 0.0f);
            triangles.push_back(Triangle(
                toVertex(vertices[3], texCoords.minU, texCoords.minV, normal), // Top-left
                toVertex(vertices[7], texCoords.maxU, texCoords.minV, normal), // Top-right
                toVertex(vertices[6], texCoords.maxU, texCoords.maxV, normal), // Bottom-right
                0.0f));
            triangles.push_back(Triangle(
                toVertex(vertices[3], texCoords.minU, texCoords.minV, normal), // Top-left
                toVertex(vertices[6], texCoords.maxU, texCoords.maxV, normal), // Bottom-right
                toVertex(vertices[2], texCoords.minU, texCoords.maxV, normal), // Bottom-left
                0.0f));
            break;
        case FaceType::BOTTOM:
            normal = glm::vec3(0.0f, -1.0f, 0.0f);
            triangles.push_back(Triangle(
                toVertex(vertices[0], texCoords.minU, texCoords.minV, normal), // Bottom-left
                toVertex(vertices[4], texCoords.maxU, texCoords.minV, normal), // Bottom-right
                toVertex(vertices[5], texCoords.maxU, texCoords.maxV, normal), // Top-right
                0.0f));
            triangles.push_back(Triangle(
                toVertex(vertices[0], texCoords.minU, texCoords.minV, normal), // Bottom-left
                toVertex(vertices[5], texCoords.maxU, texCoords.maxV, normal), // Top-right
                toVertex(vertices[1], texCoords.minU, texCoords.maxV, normal), // Top-left
                0.0f));
            break;
    }
}

void Chunk::generateChunk() {
    int seed = world.getSeed();
    PerlinNoise noise(seed);

    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 256; ++y) {
            for (int z = 0; z < 16; ++z) {
                chunk[x][y][z] = generateBlock(x + xOffset, y, z + zOffset, noise);
            }
        }
    }
}

Block Chunk::generateBlock(int x, int y, int z, PerlinNoise& noise) {
    double frequency = 0.01;
    double surfaceY = 30 + noise.noise2D(x * frequency * 0.4, z * frequency) * 70.0;
    int seaLevel = 40;

    if (y < surfaceY - 5) {
        return STONE;
    } else if (y < surfaceY) {
        return DIRT;
    } 
    // else if (y < seaLevel) {
    //     return WATER;
    // } 
    else if (y < surfaceY + 1) {
        return GRASS;
    }

    return AIR;
}
