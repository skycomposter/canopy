#ifndef CANOPY_ENGINE_H
#define CANOPY_ENGINE_H

#include <stddef.h>

// A game engine that renders frames to a provided buffer.
class CanopyEngine {
public:
    // Provides a valid buffer and its size to the engine.
    void setBufferPointer(void* pixelBuffer, size_t size);

    // Renders a frame from the current internal state and writes it to the
    // buffer.
    void renderFrame(int width, int height);

private:
    void* pixelBuffer = nullptr;
    size_t bufferSize = 0;
};

#endif // CANOPY_ENGINE_H
