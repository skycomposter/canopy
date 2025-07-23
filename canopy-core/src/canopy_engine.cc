#include "canopy_engine.h"

#include <algorithm>
#include <cmath>

#include "input_event.h"

// Size of a single pixel in the frame buffer in bytes.
const int kBytesPerPixel = 4;
// Half the length of the default square side.
const int kSquareHalfDimension = 100;

void CanopyEngine::SetBufferPointer(void *pixel_buffer, size_t size) {
    this->pixel_buffer = pixel_buffer;
    this->buffer_size = size;
}

void CanopyEngine::RenderFrame(int width, int height, double frameInterval) {
    if (width * height * kBytesPerPixel != buffer_size) {
        // TODO: throw some kind of error.
        return;
    }

    // Track the latest requested dimenions.
    frame_width = width;
    frame_height = height;

    // Move the square based on current input (arrow keys).
    int center_x = width / 2;
    int center_y = height / 2;
    if (input_state_manager[kLeftArrow].is_pressed) {
        center_x -= 300;
    }
    if (input_state_manager[kUpArrow].is_pressed) {
        center_y -= 300;
    }
    if (input_state_manager[kRightArrow].is_pressed) {
        center_x += 300;
    }
    if (input_state_manager[kBottomArrow].is_pressed) {
        center_y += 300;
    }

    DrawSquare(center_x, center_y, kSquareHalfDimension);
}

void CanopyEngine::OnInputEvent(InputEvent event) {
    input_state_manager.OnInputEvent(event);
}

void CanopyEngine::DrawSquare(int center_x, int center_y, int half_dimension) {
    int x0 = center_x - half_dimension;
    int y0 = center_y - half_dimension;
    int x1 = center_x + half_dimension;
    int y1 = center_y + half_dimension;

    unsigned int *pixel = static_cast<unsigned int *>(this->pixel_buffer);
    for (int y = 0; y < frame_height; y++) {
        for (int x = 0; x < frame_width; x++) {
            if (x >= x0 && x < x1 && y >= y0 && y < y1) {
                *pixel++ = 0xFF00FF00;
            } else {
                *pixel++ = 0xFF000000;
            }
        }
    }
}
