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

    if (neighborType == NULLBLOCK || y < 0){
        return false;
    }

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

    // Calculate ambient occlusion for this face
    float ambientOcclusion = calculateAmbientOcclusion(neighborX, neighborY, neighborZ, face);

    // Lambda function to create a Vertex with ambient occlusion
    auto toVertexWithAO = [texCoords, ambientOcclusion](const glm::vec3& pos, float u, float v, const glm::vec3& normal) {
        return Vertex{pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, u, v, ambientOcclusion};
    };

    // Set the normal for each face direction and create triangles
    glm::vec3 normal;
    switch (face) {
        case FaceType::FRONT:
            normal = glm::vec3(0.0f, 0.0f, 1.0f);
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[4], texCoords.minU, texCoords.maxV, normal),
                toVertexWithAO(vertices[5], texCoords.maxU, texCoords.maxV, normal),
                toVertexWithAO(vertices[7], texCoords.minU, texCoords.minV, normal),
                0.0f));
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[5], texCoords.maxU, texCoords.maxV, normal),
                toVertexWithAO(vertices[6], texCoords.maxU, texCoords.minV, normal),
                toVertexWithAO(vertices[7], texCoords.minU, texCoords.minV, normal),
                0.0f));
            break;
        case FaceType::BACK:
            normal = glm::vec3(0.0f, 0.0f, -1.0f);
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[1], texCoords.maxU, texCoords.maxV, normal),
                toVertexWithAO(vertices[0], texCoords.minU, texCoords.maxV, normal),
                toVertexWithAO(vertices[2], texCoords.maxU, texCoords.minV, normal),
                0.0f));
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[2], texCoords.maxU, texCoords.minV, normal),
                toVertexWithAO(vertices[0], texCoords.minU, texCoords.maxV, normal),
                toVertexWithAO(vertices[3], texCoords.minU, texCoords.minV, normal),
                0.0f));
            break;
        case FaceType::LEFT:
            normal = glm::vec3(-1.0f, 0.0f, 0.0f);
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[0], texCoords.minU, texCoords.maxV, normal),
                toVertexWithAO(vertices[7], texCoords.maxU, texCoords.minV, normal),
                toVertexWithAO(vertices[3], texCoords.minU, texCoords.minV, normal),
                0.0f));
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[0], texCoords.minU, texCoords.maxV, normal),
                toVertexWithAO(vertices[4], texCoords.maxU, texCoords.maxV, normal),
                toVertexWithAO(vertices[7], texCoords.maxU, texCoords.minV, normal),
                0.0f));
            break;
        case FaceType::RIGHT:
            normal = glm::vec3(1.0f, 0.0f, 0.0f);
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[1], texCoords.minU, texCoords.maxV, normal),
                toVertexWithAO(vertices[2], texCoords.minU, texCoords.minV, normal),
                toVertexWithAO(vertices[6], texCoords.maxU, texCoords.minV, normal),
                0.0f));
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[1], texCoords.minU, texCoords.maxV, normal),
                toVertexWithAO(vertices[6], texCoords.maxU, texCoords.minV, normal),
                toVertexWithAO(vertices[5], texCoords.maxU, texCoords.maxV, normal),
                0.0f));
            break;
        case FaceType::TOP:
            normal = glm::vec3(0.0f, 1.0f, 0.0f);
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[3], texCoords.minU, texCoords.minV, normal),
                toVertexWithAO(vertices[7], texCoords.maxU, texCoords.minV, normal),
                toVertexWithAO(vertices[6], texCoords.maxU, texCoords.maxV, normal),
                0.0f));
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[3], texCoords.minU, texCoords.minV, normal),
                toVertexWithAO(vertices[6], texCoords.maxU, texCoords.maxV, normal),
                toVertexWithAO(vertices[2], texCoords.minU, texCoords.maxV, normal),
                0.0f));
            break;
        case FaceType::BOTTOM:
            normal = glm::vec3(0.0f, -1.0f, 0.0f);
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[0], texCoords.minU, texCoords.minV, normal),
                toVertexWithAO(vertices[4], texCoords.maxU, texCoords.minV, normal),
                toVertexWithAO(vertices[5], texCoords.maxU, texCoords.maxV, normal),
                0.0f));
            triangles.push_back(Triangle(
                toVertexWithAO(vertices[0], texCoords.minU, texCoords.minV, normal),
                toVertexWithAO(vertices[5], texCoords.maxU, texCoords.maxV, normal),
                toVertexWithAO(vertices[1], texCoords.minU, texCoords.maxV, normal),
                0.0f));
            break;
    }
}

bool Chunk::isBlockSolid(int x, int y, int z) {
    BlockType neighborType = getBlockTypeAt(x, y, z);
    return neighborType != AIR && neighborType != WATER;
}

float Chunk::calculateAmbientOcclusion(int x, int y, int z, FaceType face) {
    int occlusionCount = 0;

    // Check direct neighbors based on face direction
    switch (face) {
        case FaceType::FRONT:
            occlusionCount += isBlockSolid(x, y + 1, z + 1); // Top-right
            occlusionCount += isBlockSolid(x, y - 1, z + 1); // Bottom-right
            occlusionCount += isBlockSolid(x + 1, y, z + 1); // Right
            break;
        case FaceType::BACK:
            occlusionCount += isBlockSolid(x, y + 1, z - 1); // Top-left
            occlusionCount += isBlockSolid(x, y - 1, z - 1); // Bottom-left
            occlusionCount += isBlockSolid(x - 1, y, z - 1); // Left
            break;
        case FaceType::LEFT:
            occlusionCount += isBlockSolid(x - 1, y + 1, z); // Top-left
            occlusionCount += isBlockSolid(x - 1, y - 1, z); // Bottom-left
            occlusionCount += isBlockSolid(x - 1, y, z + 1); // Forward
            break;
        case FaceType::RIGHT:
            occlusionCount += isBlockSolid(x + 1, y + 1, z); // Top-right
            occlusionCount += isBlockSolid(x + 1, y - 1, z); // Bottom-right
            occlusionCount += isBlockSolid(x + 1, y, z - 1); // Backward
            break;
        case FaceType::TOP:
            occlusionCount += isBlockSolid(x + 1, y + 1, z); // Top-right
            occlusionCount += isBlockSolid(x - 1, y + 1, z); // Top-left
            occlusionCount += isBlockSolid(x, y + 1, z + 1); // Forward
            break;
        case FaceType::BOTTOM:
            occlusionCount += isBlockSolid(x + 1, y - 1, z); // Bottom-right
            occlusionCount += isBlockSolid(x - 1, y - 1, z); // Bottom-left
            occlusionCount += isBlockSolid(x, y - 1, z - 1); // Backward
            break;
    }

    // Normalize occlusion count (0 to 1)
    return 1.0f - (occlusionCount / 3.0f);
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
    double surfaceY = 50 + noise.noise2D(x * frequency * 1.4, z * frequency) * 60.0;
    int seaLevel = 40;

    if (y < surfaceY - 5) {
        return STONE;
    } else if (y < surfaceY) {
        return DIRT;
    } 
    else if (y < seaLevel) {
        return WATER;
    } 
    else if (y < surfaceY + 1) {
        return GRASS;
    }

    return AIR;
}
