#ifndef TEXTURE_MAPPING_H
#define TEXTURE_MAPPING_H

#include "Blocknames.h" 

struct TextureCoords {
    float uMin, vMin, uMax, vMax;
};

struct BlockTexture {
    TextureCoords top;
    TextureCoords bottom;
    TextureCoords side;
};

TextureCoords getTextureCoordsForTile(int col, int row);

BlockTexture getBlockTexture(int blockType); // Updated to take int type

#endif // TEXTURE_MAPPING_H
