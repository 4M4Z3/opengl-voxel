#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <tuple>
#include <map>
#include <random>
#include <vector>
#include "Chunk.h"
#include "TextureMap.h"
#include "Block.h"
#include "Vertex.h"

class Chunk;

class World {
    int seed;

public:
    World(int seed);
    World();

    struct pair_hash {
        std::size_t operator()(const std::pair<int, int>& p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };

    std::unordered_map<std::pair<int, int>, Chunk, pair_hash> chunks;
    Block getBlock(int x, int y, int z);

    TextureMap textureMap; 

    void initializeTextureMap(); 
    int getSeed();
};

#endif // WORLD_H
