#include "World.h"


World::World(int seed) {
    this->seed = seed;
    initializeTextureMap(); 

    // Generate chunks
    for (int x = -4; x < 4; ++x) {
        for (int z = -4; z < 4; ++z) {
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
    for (int x = -4; x < 4; ++x) {
        for (int z = -4; z < 4; ++z) {
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
        for (int face = 0; face < 6; ++face) {
            int textureIndex = blockTypeInfos[blockType].textures[face];
            if (textureIndex != -1) {
                textureMap.setTexture(blockType, static_cast<FaceType>(face), textureIndex);
            }
        }
    }
}

Block World::getBlock(int x, int y, int z) {
    int chunkX = (x / 16) * 16;
    int chunkZ = (z / 16) * 16;

    auto it = chunks.find({chunkX, chunkZ});
    if (it == chunks.end()) {
        return NULLBLOCK; 
    }

    int localX = x % 16;
    int localZ = z % 16;
    if (localX < 0) localX += 16; 
    if (localZ < 0) localZ += 16;

    if (y < 0 || y >= 256) {
        return NULLBLOCK;
    }

    return it->second.chunk[localX][y][localZ];
}
