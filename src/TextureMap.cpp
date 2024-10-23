#include "TextureMap.h"
#include <iostream>

void TextureMap::setTexture(int blockType, FaceType face, int textureIndex) {
    textureMapping[blockType][face] = textureIndex;
}

TextureCoords TextureMap::getTextureCoords(int blockType, FaceType face) {
    int textureIndex = textureMapping[blockType][face];
    return calculateCoords(textureIndex);
}


TextureCoords TextureMap::calculateCoords(int textureIndex) {
    const int atlasSize = 256;
    const int tileSize = 16; 
    const int tilesPerRow = atlasSize / tileSize;

    int row = textureIndex / tilesPerRow;
    int col = textureIndex % tilesPerRow;

    float minU = col * (1.0f / tilesPerRow);
    float maxU = (col + 1) * (1.0f / tilesPerRow);
    float minV = row * (1.0f / tilesPerRow);
    float maxV = (row + 1) * (1.0f / tilesPerRow);

    const float offset = 0.001f;
    minU += offset;
    maxU -= offset;
    minV += offset;
    maxV -= offset;

    return { minU, maxU, minV, maxV };
}
