// TextureMapping.cpp
#include "TextureMapping.h"

TextureCoords getTextureCoordsForTile(int col, int row) {
    float tileSize = 1.0f / 16.0f; // Each tile is 1/16 of the atlas size

    float uMin = col * tileSize;
    float vMin = row * tileSize;
    float uMax = (col + 1) * tileSize;
    float vMax = (row + 1) * tileSize;

    return {uMin, vMin, uMax, vMax};
}

BlockTexture getBlockTexture(int blockType) { // Updated to match the header
    switch (blockType) {
        case STONE:
            return {
                getTextureCoordsForTile(1, 0), // top
                getTextureCoordsForTile(1, 0), // bottom
                getTextureCoordsForTile(1, 0)  // side
            };
        case GRASS:
            return {
                getTextureCoordsForTile(0, 1), // top
                getTextureCoordsForTile(1, 1), // bottom
                getTextureCoordsForTile(2, 1)  // side
            };
        case DIRT:
            return {
                getTextureCoordsForTile(3, 1), // top
                getTextureCoordsForTile(3, 1), // bottom
                getTextureCoordsForTile(3, 1)  // side
            };
        // Add more cases for other block types as needed
        default:
            return {
                getTextureCoordsForTile(15, 15), // top
                getTextureCoordsForTile(15, 15), // bottom
                getTextureCoordsForTile(15, 15)  // side
            };
    }
}
