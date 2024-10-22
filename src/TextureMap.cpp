#include "TextureMap.h"

void TextureMap::setTexture(int blockType, FaceType face, int textureIndex) {
    textureMapping[blockType][face] = textureIndex;
}

TextureCoords TextureMap::getTextureCoords(int blockType, FaceType face) {
    int textureIndex = textureMapping[blockType][face];
    return calculateCoords(textureIndex);
}

TextureCoords TextureMap::calculateCoords(int textureIndex) {
    int atlasSize = 256; 
    int tileSize = 16; 

    int columns = atlasSize / tileSize; 
    
    int row = textureIndex / columns;
    int col = textureIndex % columns;

    float minU = (col * tileSize) / (float)atlasSize;
    float maxU = ((col + 1) * tileSize) / (float)atlasSize;
    float minV = (row * tileSize) / (float)atlasSize;
    float maxV = ((row + 1) * tileSize) / (float)atlasSize;

    return { minU, maxU, minV, maxV };
}
