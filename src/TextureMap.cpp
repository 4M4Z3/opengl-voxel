#include "TextureMap.h"

void TextureMap::setTexture(int blockType, FaceType face, int textureIndex) {
    textureMapping[blockType][face] = textureIndex;
}

TextureCoords TextureMap::getTextureCoords(int blockType, FaceType face) {
    int textureIndex = textureMapping[blockType][face];
    return calculateCoords(textureIndex);
}

TextureCoords TextureMap::calculateCoords(int textureIndex) {
    int atlasSize = 256; // Size of the entire atlas in pixels
    int tileSize = 16;   // Size of a single texture tile in pixels

    int columns = atlasSize / tileSize;

    int row = textureIndex / columns;
    int col = textureIndex % columns;

    float offset = 0.5f / atlasSize; // Half-pixel offset to prevent bleeding
    float minU = (col * tileSize + offset) / (float)atlasSize;
    float maxU = ((col + 1) * tileSize - offset) / (float)atlasSize;
    float minV = (row * tileSize + offset) / (float)atlasSize;
    float maxV = ((row + 1) * tileSize - offset) / (float)atlasSize;

    return { minU, maxU, minV, maxV };
}
