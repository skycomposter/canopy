#include "canopy_engine.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "input_event.h"

const int kSquareHalfDimension = 100;

void CanopyEngine::SetBufferPointer(void *pixel_buffer, size_t size) {
    this->pixel_buffer = pixel_buffer;
    this->buffer_size = size;
}

void CanopyEngine::RenderFrame(int width, int height) {
    if (latest_pointer_position == nullptr) {
        // Only start drawing once the pointer has entered the window once.
        return;
    }

    frame_width = width;
    frame_height = height;

    DrawSquare(latest_pointer_position->x, latest_pointer_position->y,
        kSquareHalfDimension);
}

void CanopyEngine::OnInputEvent(InputEvent event) {
    if (event.type != POINTER_MOVED) {
        // Only accept POINTER_MOVED events for now.
        return;
    }

    if (latest_pointer_position == nullptr) {
        latest_pointer_position = new Point2D();
    }
    latest_pointer_position->x = event.position.x;
    latest_pointer_position->y = event.position.y;
}

void CanopyEngine::DrawSquare(int center_x, int center_y, int half_size) {
    int x0 = center_x - half_size;
    int y0 = center_y - half_size;
    int x1 = center_x + half_size;
    int y1 = center_y + half_size;

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
