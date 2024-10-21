#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include <unordered_map>

// Enum to represent different faces of a block
enum FaceType {
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

// Structure to store texture coordinates
struct TextureCoords {
    float minU, maxU;
    float minV, maxV;
};

// Class to manage texture mapping for different block types
class TextureMap {
public:
    void setTexture(int blockType, FaceType face, int textureIndex);
    TextureCoords getTextureCoords(int blockType, FaceType face);

private:
    std::unordered_map<int, std::unordered_map<FaceType, int>> textureMapping;
    TextureCoords calculateCoords(int textureIndex); // Calculate texture coordinates based on index
};

#endif // TEXTURE_MAP_H
