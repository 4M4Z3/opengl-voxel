#include "Block.h"

Block::Block() : type(AIR) {}

Block::Block(BlockType type) : type(type) {}

MeshType Block::getMeshType() const {
    return blockTypeInfos[type].meshType;
}

int Block::getTextureIndex(FaceType face) const {
    if (type < 0 || type >= sizeof(blockTypeInfos) / sizeof(BlockTypeInfo)) {
        return -1; 
    }
    return blockTypeInfos[type].textures[face];
}

