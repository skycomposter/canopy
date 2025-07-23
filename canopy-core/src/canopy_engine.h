#ifndef CANOPY_ENGINE_H_
#define CANOPY_ENGINE_H_

#include <stddef.h>

#include <queue>

#include "input.h"
#include "input_event.h"
#include "util/primitives.h"

// A game engine that renders frames to a provided buffer.
class CanopyEngine {
public:
    CanopyEngine();

    // Provides a valid buffer and its size to the engine.
    void SetBufferPointer(void *pixel_buffer, size_t size);

    // Processes an input event and updates the internal state accordingly.
    void OnInputEvent(InputEvent event);

    // Renders a frame from the current internal state and writes it to the
    // buffer.
    void RenderFrame(int width, int height, double frameInterval);

private:
    // Bookkeeping for user input.
    InputStateManager input_state_manager;

    // Shared frame buffer in which frames are rendered.
    void *pixel_buffer = nullptr;
    // Allocated size of the pixel_buffer.
    size_t buffer_size = 0;
    // Width of the next requested frame.
    int frame_width = 0;
    // Height of the next requested frame.
    int frame_height = 0;
    // The current center of the moving rectangle, expressed as a percentage of
    // the frame's width and height.
    Point2D rect_position;

    // Draws a rectangle with the given center position, half dimensions, and
    // color.
    void DrawRect(Point2D position, Size2D half_dimensions, Color color);

    // Draws a rectangle with the given bounds and color.
    void DrawRectWithBounds(int left, int top, int right, int bottom,
                            Color color);
};

#endif  // CANOPY_ENGINE_H_
