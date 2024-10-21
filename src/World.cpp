#include "World.h"
#include "Block.h"
#include "Vertex.h"
#include <random>
#include <vector>

World::World(int seed){
    this->seed = seed;
}

World::World() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, std::numeric_limits<int>::max());
    seed = dis(gen);

    for (int x = -4; x < 4; ++x) {
        for (int z = -4; z < 4; ++z) {
            int chunkX = x * 16;
            int chunkZ = z * 16;

            Chunk c = Chunk(chunkX, chunkZ, this);
            chunks.insert({{chunkX, chunkZ}, c});
        }
    }

    for (auto& [key, chunk] : chunks) {
        chunk.initializeMesh();
    }
}

Block World::getBlock(int x, int y, int z) {
    int chunkX = (x / 16) * 16;
    int chunkZ = (z / 16) * 16;

    auto it = chunks.find({chunkX, chunkZ});
    if (it == chunks.end()) {
        return -1; 
    }

    int localX = x % 16;
    int localZ = z % 16;
    if (localX < 0) localX += 16; 
    if (localZ < 0) localZ += 16;

    if (y < 0 || y >= 256) {
        return -1;
    }

    return it->second.chunk[localX][y][localZ];
}
