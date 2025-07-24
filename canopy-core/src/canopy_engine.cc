#include "canopy_engine.h"

#include <algorithm>
#include <cmath>

#include "input/input.h"
#include "input/input_event.h"
#include "util/primitives.h"
#include "util/utils.h"

// Size of a single pixel in the frame buffer in bytes.
const int kBytesPerPixel = 4;

// Scene colors.
const Color kArenaColor = 0xFF000000;
const Color kBallColor = 0xFF00FF00;
const Color kBorderColor = 0xFF00FF00;

// Arena settings
const Size2D kBorderSize = {30, 30};

// Ball settings.
const Size2D kBallSize = {32, 32};
const Velocity2D kBallVelocity = {800, 800};

void CanopyEngine::SetBufferPointer(void *pixel_buffer, size_t size) {
    this->pixel_buffer = pixel_buffer;
    this->buffer_size = size;
}

void CanopyEngine::RenderFrame(unsigned long width, unsigned long height,
                               double frameInterval) {
    if (width * height * kBytesPerPixel != buffer_size) {
        // TODO: throw some kind of error.
        return;
    }

    // Track the latest requested dimenions.
    frame_width = width;
    frame_height = height;

    // If this is the first frame, put the ball in the middle.
    if (ball_position.x == -1 || ball_position.y == -1) {
        ball_position = {width / 2.0, height / 2.0};
    }

    // Update the current ball position based on the latest input.
    if (input_state_manager[kLeftArrow].is_pressed) {
        ball_position.x -= kBallVelocity.x * frameInterval; 
    }
    if (input_state_manager[kUpArrow].is_pressed) {
        ball_position.y -= kBallVelocity.y * frameInterval; 
    }
    if (input_state_manager[kRightArrow].is_pressed) {
        ball_position.x += kBallVelocity.x * frameInterval; 
    }
    if (input_state_manager[kDownArrow].is_pressed) {
        ball_position.y += kBallVelocity.y * frameInterval; 
    }
    double horizontal_padding = kBorderSize.width + kBallSize.width / 2;
    double vertical_padding = kBorderSize.height + kBallSize.height / 2;
    ball_position.x = clamp(ball_position.x, horizontal_padding,
        frame_width - horizontal_padding);
    ball_position.y = clamp(ball_position.y, vertical_padding,
        frame_height - vertical_padding);

    DrawRectWithBounds(0.0, 0.0, frame_width, frame_height, kBorderColor);
    DrawRectWithBounds(
        kBorderSize.width, kBorderSize.height, frame_width - kBorderSize.width,
        frame_height - kBorderSize.height, kArenaColor);
    DrawRectAbsolute(ball_position, kBallSize, kBallColor);
}

void CanopyEngine::OnInputEvent(InputEvent event) {
    input_state_manager.OnInputEvent(event);
}

void CanopyEngine::DrawRectAbsolute(Point2D position, Size2D dimensions,
                                    Color color) {
    int left = position.x - dimensions.width / 2;
    int top = position.y - dimensions.height / 2;
    int right = position.x + dimensions.width / 2;
    int bottom = position.y + dimensions.height / 2;
    DrawRectWithBounds(left, top, right, bottom, color);
}

void CanopyEngine::DrawRectRelative(Point2D position, Size2D dimensions,
                                    Color color) {
    int left = (position.x - dimensions.width / 2) * frame_width;
    int top = (position.y - dimensions.height / 2) * frame_height;
    int right = (position.x + dimensions.width / 2) * frame_width;
    int bottom = (position.y + dimensions.height / 2) * frame_height;
    DrawRectWithBounds(left, top, right, bottom, color);
}

void CanopyEngine::DrawRectWithBounds(int left, int top, int right, int bottom,
                                      Color color) {
    for (int y = top; y < bottom; y++) {
        unsigned int *pixel = static_cast<unsigned int *>(this->pixel_buffer)
            + left + y * frame_width;
        for (int x = left; x < right; x++) {
            *pixel++ = color;
        }
    }
}
