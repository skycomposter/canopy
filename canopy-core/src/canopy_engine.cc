#include "canopy_engine.h"

#include <algorithm>
#include <cmath>

#include "input.h"
#include "input_event.h"
#include "util/primitives.h"
#include "util/utils.h"

// Size of a single pixel in the frame buffer in bytes.
const int kBytesPerPixel = 4;
// Background color (black).
const Color kBackgroundColor = 0xFF000000;
// Color of the moving rectangle (green).
const Color kRectColor = 0xFF00FF00;
// Half the length of each rectangle dimension as a percentage of the matching
// frame dimension.
const double kRectHalfSize = 0.02;
// Movement speed of the rectangle in units per second.
const double kRectMovementSpeed = 0.5;
// Starting position of the rectangle as a percentage of the matching frame
// dimension.
const double kRectStartingPosition = 0.5;

CanopyEngine::CanopyEngine() {
    rect_position.x = kRectStartingPosition;
    rect_position.y = kRectStartingPosition;
}

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

    // Update the current position based on the latest input.
    if (input_state_manager[kLeftArrow].is_pressed) {
        rect_position.x -= kRectMovementSpeed * frameInterval; 
    }
    if (input_state_manager[kUpArrow].is_pressed) {
        rect_position.y -= kRectMovementSpeed * frameInterval; 
    }
    if (input_state_manager[kRightArrow].is_pressed) {
        rect_position.x += kRectMovementSpeed * frameInterval; 
    }
    if (input_state_manager[kDownArrow].is_pressed) {
        rect_position.y += kRectMovementSpeed * frameInterval; 
    }
    rect_position.x =
        clamp(rect_position.x, kRectHalfSize, 1.0 - kRectHalfSize);
    rect_position.y =
        clamp(rect_position.y, kRectHalfSize, 1.0 - kRectHalfSize);

    // TODO: this should be defined once somewhere else and reused, but it
    //   won't stick around for long so it's OK.
    Size2D size;
    size.width = kRectHalfSize;
    size.height = kRectHalfSize;

    DrawRect(rect_position, size, kRectColor);
}

void CanopyEngine::OnInputEvent(InputEvent event) {
    input_state_manager.OnInputEvent(event);
}

void CanopyEngine::DrawRect(Point2D position, Size2D half_dimensions,
                            Color color) {
    int left = (position.x - half_dimensions.width) * frame_width;
    int top = (position.y- half_dimensions.height) * frame_height;
    int right = (position.x + half_dimensions.width) * frame_width;
    int bottom = (position.y + half_dimensions.height) * frame_height;
    DrawRectWithBounds(left, top, right, bottom, color);
}

void CanopyEngine::DrawRectWithBounds(int left, int top, int right, int bottom,
                                      Color color) {
    unsigned int *pixel = static_cast<unsigned int *>(this->pixel_buffer);
    for (int y = 0; y < frame_height; y++) {
        for (int x = 0; x < frame_width; x++) {
            if (x >= left && x < right && y >= top && y < bottom) {
                *pixel++ = color;
            } else {
                *pixel++ = kBackgroundColor;
            }
        }
    }
}
