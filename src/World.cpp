#include "World.h"
#include <iostream>

World::World(int seed) {
    this->seed = seed;
    initializeTextureMap(); 

    // Generate chunks
    for (int x = -10; x < 10; ++x) {
        for (int z = -10; z < 10; ++z) {
            int chunkX = x * 16;
            int chunkZ = z * 16;

            Chunk c = Chunk(chunkX, chunkZ, *this);
            chunks.insert({{chunkX, chunkZ}, c});
        }
    }

    // Initialize mesh for each chunk
    for (auto& [key, chunk] : chunks) {
        chunk.initializeMesh();
    }
}

World::World() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, std::numeric_limits<int>::max());
    seed = dis(gen);
    initializeTextureMap(); 

    // Generate chunks
    for (int x = -12; x < 12; ++x) {
        for (int z = -12; z < 12; ++z) {
            int chunkX = x * 16;
            int chunkZ = z * 16;

            Chunk c = Chunk(chunkX, chunkZ, *this);
            chunks.insert({{chunkX, chunkZ}, c});
        }
    }

    for (auto& [key, chunk] : chunks) {
        chunk.initializeMesh();
    }
}

int World::getSeed(){
    return seed;
}

void World::initializeTextureMap() {
    for (int blockType = 0; blockType < sizeof(blockTypeInfos) / sizeof(BlockTypeInfo); ++blockType) {
        // Skip setting textures for AIR or NULLBLOCK
        if (blockType == AIR || blockType == NULLBLOCK) {
            continue;
        }
        for (int face = 0; face < 6; ++face) {
            int textureIndex = blockTypeInfos[blockType].textures[face];
            if (textureIndex != -1) {
                textureMap.setTexture(blockType, static_cast<FaceType>(face), textureIndex);
            } else {
            }
        }
    }
}



Block World::getBlock(int x, int y, int z) {
    int chunkX = (x < 0 ? ((x + 1) / 16 - 1) * 16 : (x / 16) * 16);
    int chunkZ = (z < 0 ? ((z + 1) / 16 - 1) * 16 : (z / 16) * 16);

    auto it = chunks.find({chunkX, chunkZ});
    if (it == chunks.end()) {
        return Block(NULLBLOCK);
    }

    int localX = (x % 16 + 16) % 16;
    int localZ = (z % 16 + 16) % 16;

    if (y < 0 || y >= 256) {
        return Block(NULLBLOCK); 
    }

    return it->second.chunk[localX][y][localZ];
}




std::vector<Triangle> World::getVisibleTriangles(const Camera& camera) const {
    std::vector<Triangle> visibleTriangles; 
    visibleTriangles.clear(); 

    glm::vec3 cameraPosition = camera.getPosition();
    float viewDistance = 150.0f;

    for (const auto& [key, chunk] : chunks) {
        glm::vec3 chunkCenter = glm::vec3(key.first + 8, 0, key.second + 8);

        if (glm::distance(cameraPosition, chunkCenter) < viewDistance) {
            for (const Triangle& tri : chunk.triangles) {
                visibleTriangles.push_back(tri);
            }
        }
    }

    return visibleTriangles;
}

Chunk* World::getChunk(int chunkX, int chunkZ) {
    auto it = chunks.find({chunkX, chunkZ});
    if (it != chunks.end()) {
        return &it->second;
    }
    return nullptr;
}
