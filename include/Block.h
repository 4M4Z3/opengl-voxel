#ifndef BLOCK_H
#define BLOCK_H

#include "BlockTypes.h"

class Block {
public:
    BlockType type;

    Block();
    Block(BlockType type);
    
    MeshType getMeshType() const;
    int getTextureIndex(FaceType face) const;
};

#endif // BLOCK_H
