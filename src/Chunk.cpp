// Chunk.cpp
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
                if (block.type != AIR) {
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

                        // Check if each face should be rendered
                        if (isBlockAirOrOutOfBounds(x + 1, y, z)) {
                            addFaceTriangles(vertices, FaceType::RIGHT, x + 1, y, z, block);
                        }
                        if (isBlockAirOrOutOfBounds(x - 1, y, z)) {
                            addFaceTriangles(vertices, FaceType::LEFT, x - 1, y, z, block);
                        }
                        if (isBlockAirOrOutOfBounds(x, y + 1, z)) {
                            addFaceTriangles(vertices, FaceType::TOP, x, y + 1, z, block);
                        }
                        if (isBlockAirOrOutOfBounds(x, y - 1, z)) {
                            addFaceTriangles(vertices, FaceType::BOTTOM, x, y - 1, z, block);
                        }
                        if (isBlockAirOrOutOfBounds(x, y, z + 1)) {
                            addFaceTriangles(vertices, FaceType::FRONT, x, y, z + 1, block);
                        }
                        if (isBlockAirOrOutOfBounds(x, y, z - 1)) {
                            addFaceTriangles(vertices, FaceType::BACK, x, y, z - 1, block);
                        }
                    }
                }
            }
        }
    }
}

bool Chunk::isBlockAirOrOutOfBounds(int x, int y, int z) {
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
            return true;
        }

        return neighborChunk->chunk[localX][y][localZ].type == AIR;
    }

    if (y < 0 || y >= 256) {
        return true;
    }

    return chunk[x][y][z].type == AIR;
}




void Chunk::addFaceTriangles(glm::vec3* vertices, FaceType face, int neighborX, int neighborY, int neighborZ, Block block) {
    int worldX = neighborX + xOffset;
    int worldZ = neighborZ + zOffset;

    if (world.getBlock(worldX, neighborY, worldZ).type == AIR) {
    int textureIndex = blockTypeInfos[block.type].textures[face];
    TextureCoords texCoords = world.textureMap.getTextureCoords(block.type, face);


        auto toVertex = [texCoords](const glm::vec3& pos, float u, float v) {
            return Vertex{pos.x, pos.y, pos.z, 1.0f, 1.0f, 1.0f, u, v};
        };

        switch (face) {
            case FaceType::FRONT:
                triangles.push_back(Triangle(
                    toVertex(vertices[4], texCoords.minU, texCoords.minV),
                    toVertex(vertices[5], texCoords.maxU, texCoords.minV),
                    toVertex(vertices[6], texCoords.maxU, texCoords.maxV), 0.0f));
                triangles.push_back(Triangle(
                    toVertex(vertices[4], texCoords.minU, texCoords.minV),
                    toVertex(vertices[6], texCoords.maxU, texCoords.maxV),
                    toVertex(vertices[7], texCoords.minU, texCoords.maxV), 0.0f));
                break;
            case FaceType::BACK:
                triangles.push_back(Triangle(
                    toVertex(vertices[0], texCoords.minU, texCoords.minV),
                    toVertex(vertices[2], texCoords.maxU, texCoords.maxV),
                    toVertex(vertices[1], texCoords.maxU, texCoords.minV), 0.0f));
                triangles.push_back(Triangle(
                    toVertex(vertices[0], texCoords.minU, texCoords.minV),
                    toVertex(vertices[3], texCoords.minU, texCoords.maxV),
                    toVertex(vertices[2], texCoords.maxU, texCoords.maxV), 0.0f));
                break;
            case FaceType::LEFT:
                triangles.push_back(Triangle(
                    toVertex(vertices[0], texCoords.minU, texCoords.minV),
                    toVertex(vertices[7], texCoords.maxU, texCoords.maxV),
                    toVertex(vertices[4], texCoords.maxU, texCoords.minV), 0.0f));
                triangles.push_back(Triangle(
                    toVertex(vertices[0], texCoords.minU, texCoords.minV),
                    toVertex(vertices[3], texCoords.minU, texCoords.maxV),
                    toVertex(vertices[7], texCoords.maxU, texCoords.maxV), 0.0f));
                break;
            case FaceType::RIGHT:
                triangles.push_back(Triangle(
                    toVertex(vertices[1], texCoords.minU, texCoords.minV),
                    toVertex(vertices[6], texCoords.maxU, texCoords.maxV),
                    toVertex(vertices[5], texCoords.maxU, texCoords.minV), 0.0f));
                triangles.push_back(Triangle(
                    toVertex(vertices[1], texCoords.minU, texCoords.minV),
                    toVertex(vertices[2], texCoords.minU, texCoords.maxV),
                    toVertex(vertices[6], texCoords.maxU, texCoords.maxV), 0.0f));
                break;
            case FaceType::TOP:
                triangles.push_back(Triangle(
                    toVertex(vertices[3], texCoords.minU, texCoords.minV),
                    toVertex(vertices[6], texCoords.maxU, texCoords.maxV),
                    toVertex(vertices[7], texCoords.maxU, texCoords.minV), 0.0f));
                triangles.push_back(Triangle(
                    toVertex(vertices[3], texCoords.minU, texCoords.minV),
                    toVertex(vertices[2], texCoords.minU, texCoords.maxV),
                    toVertex(vertices[6], texCoords.maxU, texCoords.maxV), 0.0f));
                break;
            case FaceType::BOTTOM:
                triangles.push_back(Triangle(
                    toVertex(vertices[0], texCoords.minU, texCoords.minV),
                    toVertex(vertices[5], texCoords.maxU, texCoords.maxV),
                    toVertex(vertices[4], texCoords.maxU, texCoords.minV), 0.0f));
                triangles.push_back(Triangle(
                    toVertex(vertices[0], texCoords.minU, texCoords.minV),
                    toVertex(vertices[1], texCoords.minU, texCoords.maxV),
                    toVertex(vertices[5], texCoords.maxU, texCoords.maxV), 0.0f));
                break;
        }
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
    double surfaceY = 70.0 + noise.noise2D(x * frequency * 0.4, z * frequency) * 70.0;
    return (y < surfaceY) ? STONE : AIR;

    return (y < abs(x) / 10 + 1) ? STONE : AIR;

    // if (y < 40 + 35*sin(x*0.1)){
    //     if ((abs(x) % 16) < 5){
    //         return STONE;
    //     }
    //     else if ((abs(x) % 16) < 10){
    //         return GRASS;
    //     }

    //     return WOOD;
    // }
    // return AIR;
}
