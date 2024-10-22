// Chunk.cpp
#include "Chunk.h"
#include <glm/glm.hpp>

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
                            addFaceTriangles(vertices, FaceType::RIGHT, x + 1, y, z);
                        }
                        if (isBlockAirOrOutOfBounds(x - 1, y, z)) {
                            addFaceTriangles(vertices, FaceType::LEFT, x - 1, y, z);
                        }
                        if (isBlockAirOrOutOfBounds(x, y + 1, z)) {
                            addFaceTriangles(vertices, FaceType::TOP, x, y + 1, z);
                        }
                        if (isBlockAirOrOutOfBounds(x, y - 1, z)) {
                            addFaceTriangles(vertices, FaceType::BOTTOM, x, y - 1, z);
                        }
                        if (isBlockAirOrOutOfBounds(x, y, z + 1)) {
                            addFaceTriangles(vertices, FaceType::FRONT, x, y, z + 1);
                        }
                        if (isBlockAirOrOutOfBounds(x, y, z - 1)) {
                            addFaceTriangles(vertices, FaceType::BACK, x, y, z - 1);
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

    // Convert local chunk coordinates to world coordinates
    int worldX = x + xOffset;
    int worldZ = z + zOffset;

    // Handle coordinates that are out of bounds for the current chunk
    if (x < 0 || x >= 16 || z < 0 || z >= 16) {
        int neighborChunkX = (worldX / 16) * 16;
        int neighborChunkZ = (worldZ / 16) * 16;

        // Correct for negative modulo results to ensure coordinates are within [0, 15]
        localX = ((x % 16) + 16) % 16;
        localZ = ((z % 16) + 16) % 16;

        Chunk* neighborChunk = world.getChunk(neighborChunkX, neighborChunkZ);
        if (!neighborChunk) {
            // If the neighboring chunk is not loaded, consider it air
            return true;
        }

        return neighborChunk->chunk[localX][y][localZ].type == AIR;
    }

    // Check if the block is air within the current chunk
    if (y < 0 || y >= 256) {
        return true;
    }

    return chunk[x][y][z].type == AIR;
}



void Chunk::addFaceTriangles(glm::vec3* vertices, FaceType face, int neighborX, int neighborY, int neighborZ) {
    int worldX = neighborX + xOffset;
    int worldZ = neighborZ + zOffset;

    if (world.getBlock(worldX, neighborY, worldZ).type == AIR) {
        float r = 1.0f, g = 1.0f, b = 1.0f;
        int blockType = chunk[neighborX % 16][neighborY][neighborZ % 16].type;

        auto toVertex = [r, g, b, this, blockType, face](const glm::vec3& pos) {
            TextureCoords texCoords = world.textureMap.getTextureCoords(blockType, face);
            return Vertex{pos.x, pos.y, pos.z, r, g, b, texCoords.minU, texCoords.minV};
        };

        // Add the corresponding two triangles for the face
        switch (face) {
            case FaceType::FRONT:
                triangles.push_back(Triangle(toVertex(vertices[4]), toVertex(vertices[5]), toVertex(vertices[6]), 0.0f));
                triangles.push_back(Triangle(toVertex(vertices[4]), toVertex(vertices[6]), toVertex(vertices[7]), 0.0f));
                break;
            case FaceType::BACK:
                triangles.push_back(Triangle(toVertex(vertices[0]), toVertex(vertices[2]), toVertex(vertices[1]), 0.0f));
                triangles.push_back(Triangle(toVertex(vertices[0]), toVertex(vertices[3]), toVertex(vertices[2]), 0.0f));
                break;
            case FaceType::LEFT:
                triangles.push_back(Triangle(toVertex(vertices[0]), toVertex(vertices[7]), toVertex(vertices[4]), 0.0f));
                triangles.push_back(Triangle(toVertex(vertices[0]), toVertex(vertices[3]), toVertex(vertices[7]), 0.0f));
                break;
            case FaceType::RIGHT:
                triangles.push_back(Triangle(toVertex(vertices[1]), toVertex(vertices[6]), toVertex(vertices[5]), 0.0f));
                triangles.push_back(Triangle(toVertex(vertices[1]), toVertex(vertices[2]), toVertex(vertices[6]), 0.0f));
                break;
            case FaceType::TOP:
                triangles.push_back(Triangle(toVertex(vertices[3]), toVertex(vertices[6]), toVertex(vertices[7]), 0.0f));
                triangles.push_back(Triangle(toVertex(vertices[3]), toVertex(vertices[2]), toVertex(vertices[6]), 0.0f));
                break;
            case FaceType::BOTTOM:
                triangles.push_back(Triangle(toVertex(vertices[0]), toVertex(vertices[5]), toVertex(vertices[4]), 0.0f));
                triangles.push_back(Triangle(toVertex(vertices[0]), toVertex(vertices[1]), toVertex(vertices[5]), 0.0f));
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
    double surfaceY = 40.0 + noise.noise2D(x * frequency, z * frequency) * 20.0;
    return (y < surfaceY) ? STONE : AIR;

    // return (y < 40 + 5 * sin(x / M_PI)) ? WOOD : AIR;
}