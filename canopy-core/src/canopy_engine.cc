#include "canopy_engine.h"

#include <iostream>

void CanopyEngine::setBufferPointer(void* pixelBuffer, size_t size) {
    this->pixelBuffer = pixelBuffer;
    this->bufferSize = size;
}

void CanopyEngine::renderFrame(int width, int height) {
    unsigned int* pixel = static_cast<unsigned int*>(this->pixelBuffer);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned int color = 255 << 24; // a
            color |= ((x + y) * 255 / (height + width - 2)) << 16; // b
            color |= (y * 255 / (height - 1)) << 8; // g
            color |= x * 255 / (width - 1); // r
            *pixel++ = color;
        }
    }
}
