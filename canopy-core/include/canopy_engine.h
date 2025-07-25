#ifndef CANOPY_ENGINE_H_
#define CANOPY_ENGINE_H_

#include <queue>
#include <stddef.h>

#include "input/input.h"
#include "input/input_event.h"
#include "state/game_state.h"
#include "util/primitives.h"

// A game engine that renders frames to a provided buffer.
class CanopyEngine {
public:
    // Provides a valid buffer and its size to the engine.
    void SetBufferPointer(void *pixel_buffer, size_t size);

    // Processes an input event and updates the internal state accordingly.
    void OnInputEvent(InputEvent event);

    // Renders a frame from the current internal state and writes it to the
    // buffer.
    void RenderFrame(unsigned long width, unsigned long height,
                     double frame_interval);

private:
    // Bookkeeping for the game state.
    GameState game_state;
    // Bookkeeping for user input.
    InputStateManager input_state_manager;

    // Shared frame buffer in which frames are rendered.
    void *pixel_buffer = nullptr;
    // Allocated size of the pixel_buffer.
    size_t buffer_size = 0;
    // Width of the next requested frame.
    unsigned long frame_width = 0;
    // Height of the next requested frame.
    unsigned long frame_height = 0;

    // Draws a rectangle with the given center position, dimensions, and color.
    void DrawRectAbsolute(Point2D position, Size2D dimensions, Color color);
    // Draws a rectangle with the given center position and dimensions
    // (expressed as percentages of the frame dimensions), and color of the
    // frame.
    void DrawRectRelative(Point2D position, Size2D dimensions, Color color);

    // Draws a rectangle with the given bounds and color.
    void DrawRectWithBounds(int left, int top, int right, int bottom,
                            Color color);
};

#endif  // CANOPY_ENGINE_H_
