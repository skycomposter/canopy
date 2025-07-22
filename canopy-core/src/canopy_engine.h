#ifndef CANOPY_ENGINE_H
#define CANOPY_ENGINE_H

#include <queue>
#include <stddef.h>

#include "input_event.h"

// A game engine that renders frames to a provided buffer.
class CanopyEngine {
public:
    ~CanopyEngine() {
        delete latest_pointer_position;
    }

    // Provides a valid buffer and its size to the engine.
    void SetBufferPointer(void *pixel_buffer, size_t size);

    // Renders a frame from the current internal state and writes it to the
    // buffer.
    void RenderFrame(int width, int height);

    // Processes an input event and updates the internal state accordingly.
    void OnInputEvent(InputEvent event);

private:
    void *pixel_buffer = nullptr;
    size_t buffer_size = 0;
    int frame_width = 0;
    int frame_height = 0;

    Point2D *latest_pointer_position = nullptr;

    // Draws a square centered at the given coordinates and with the given
    // half dimension.
    void DrawSquare(int center_x, int center_y, int half_dimension);
};

#endif // CANOPY_ENGINE_H
