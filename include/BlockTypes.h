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
    int textures[6]; // Array to hold texture indices for each face
    MeshType meshType;
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
    { { -1, -1, -1, -1, -1, -1 }, MeshType::CUBE }, // AIR (should not render)
    { { 1, 1, 1, 1, 1, 1 }, MeshType::CUBE },       // STONE
    { { 2, 2, 2, 2, 2, 2 }, MeshType::CUBE },       // DIRT
    { { 3, 3, 3, 3, 0, 3 }, MeshType::CUBE },       // GRASS
    { { 195, 195, 195, 195, 195, 195 }, MeshType::CUBE }, // WATER
    { { 20, 20, 20, 20, 21, 21 }, MeshType::CUBE }, // WOOD
    { { 52, 52, 52, 52, 52, 52 }, MeshType::CUBE }, // LEAVES
};


#endif // BLOCKTYPES_H
