#include "World.h"
#include "Block.h"
#include "Vertex.h"
#include "Blocknames.h"
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

for (int x = -2; x < 4; ++x) {
    for (int z = -2; z < 4; ++z) {
        Chunk c = Chunk(x * 16, z * 16);
        chunks.insert({{x, z}, c});
    }
}
}

Block World::getBlock(int x, int y, int z) {
    int chunkX = x - (x % 16);
    int chunkY = z - (z % 16);

    auto it = chunks.find({chunkX, chunkY});
    if (it == chunks.end()) {
        return NULLBLOCK;
    }

    return it->second.chunk[x][y][z];
}