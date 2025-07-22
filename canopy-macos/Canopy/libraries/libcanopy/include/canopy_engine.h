#ifndef CANOPY_ENGINE_H
#define CANOPY_ENGINE_H

#include <queue>
#include <stddef.h>

#include "input.h"
#include "input_event.h"

// A game engine that renders frames to a provided buffer.
class CanopyEngine {
public:
    // Provides a valid buffer and its size to the engine.
    void SetBufferPointer(void *pixel_buffer, size_t size);

    // Processes an input event and updates the internal state accordingly.
    void OnInputEvent(InputEvent event);

    // Renders a frame from the current internal state and writes it to the
    // buffer.
    void RenderFrame(int width, int height);

private:
    // Shared frame buffer in which frames are rendered.
    void *pixel_buffer = nullptr;
    // Allocated size of the pixel_buffer.
    size_t buffer_size = 0;
    // Width of the next requested frame.
    int frame_width = 0;
    // Height of the next requested frame.
    int frame_height = 0;

    // Bookkeeping for user input.
    InputStateManager input_state_manager;

    // Draws a square centered at the given coordinates and with the given
    // half dimension.
    void DrawSquare(int center_x, int center_y, int half_dimension);
};

#endif // CANOPY_ENGINE_H
