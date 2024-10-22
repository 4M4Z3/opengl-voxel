#include "World.h"


World::World(int seed) {
    this->seed = seed;
    initializeTextureMap(); 

    // Generate chunks
    for (int x = -4; x < 4; ++x) {
        for (int z = -4; z < 4; ++z) {
            int chunkX = x * 16;
            int chunkZ = z * 16;

            Chunk c = Chunk(chunkX, chunkZ, this);
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

            Chunk c = Chunk(chunkX, chunkZ, this);
            chunks.insert({{chunkX, chunkZ}, c});
        }
    }

    // Initialize mesh for each chunk
    for (auto& [key, chunk] : chunks) {
        chunk.initializeMesh();
    }
}

void World::initializeTextureMap() {
    textureMap.setTexture(STONE, FRONT, 1);  // Texture 0 for the front face of stone
    textureMap.setTexture(STONE, BACK, 1);   // Texture 0 for the back face of stone
    textureMap.setTexture(STONE, LEFT, 1);   // Texture 0 for the left face of stone
    textureMap.setTexture(STONE, RIGHT, 1);  // Texture 0 for the right face of stone
    textureMap.setTexture(STONE, TOP, 1);    // Texture 1 for the top face of stone
    textureMap.setTexture(STONE, BOTTOM, 1); // Texture 2 for the bottom face of stone

    textureMap.setTexture(DIRT, FRONT, 2);   // Texture 3 for the front face of dirt
    textureMap.setTexture(DIRT, BACK, 2);    // Texture 3 for the back face of dirt
    textureMap.setTexture(DIRT, LEFT, 2);    // Texture 3 for the left face of dirt
    textureMap.setTexture(DIRT, RIGHT, 2);   // Texture 3 for the right face of dirt
    textureMap.setTexture(DIRT, TOP, 2);     // Texture 4 for the top face of dirt
    textureMap.setTexture(DIRT, BOTTOM, 2);  // Texture 5 for the bottom face of dirt

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
