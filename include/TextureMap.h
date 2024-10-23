#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include "Block.h"
#include <unordered_map>

struct TextureCoords {
    float minU, maxU;
    float minV, maxV;
};

class TextureMap {
public:
    void setTexture(int blockType, FaceType face, int textureIndex);

// private:
    TextureCoords getTextureCoords(int blockType, FaceType face);
    std::unordered_map<int, std::unordered_map<FaceType, int>> textureMapping;
    TextureCoords calculateCoords(int textureIndex); 
};

#endif // TEXTURE_MAP_H
