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
    TextureCoords getTextureCoords(int blockType, FaceType face);

private:
    std::unordered_map<int, std::unordered_map<FaceType, int>> textureMapping;
    TextureCoords calculateCoords(int textureIndex); // Calculate texture coordinates based on index
};

#endif // TEXTURE_MAP_H
