#ifndef BLOCKTYPES_H
#define BLOCKTYPES_H

enum MeshType {
    CUBE = 0, 
};

enum FaceType {
    FRONT = 0,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

struct BlockTypeInfo {
    int textures[6];
    MeshType meshType;
    bool isTransparent; 
};


enum BlockType {
    NULLBLOCK = -1,
    AIR,
    STONE,
    DIRT,
    GRASS,
    WATER,
    WOOD,
    LEAVES,
};

const BlockTypeInfo blockTypeInfos[] = {
    { { -1, -1, -1, -1, -1, -1 }, MeshType::CUBE, true },  // AIR (should not render)
    { { 1, 1, 1, 1, 1, 1 }, MeshType::CUBE, false },       // STONE
    { { 2, 2, 2, 2, 2, 2 }, MeshType::CUBE, false },       // DIRT
    { { 3, 3, 3, 3, 0, 3 }, MeshType::CUBE, false },       // GRASS
    { { 207, 207, 207, 207, 207, 207 }, MeshType::CUBE, true }, // WATER
    { { 20, 20, 20, 20, 21, 21 }, MeshType::CUBE, false }, // WOOD
    { { 52, 52, 52, 52, 52, 52 }, MeshType::CUBE, true },  // LEAVES
};

#endif // BLOCKTYPES_H
