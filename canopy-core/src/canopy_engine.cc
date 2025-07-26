#include "canopy_engine.h"

#include <algorithm>
#include <cmath>

#include "input/input.h"
#include "input/input_event.h"
#include "settings/settings.h"
#include "util/primitives.h"
#include "util/utils.h"

// Size of a single pixel in the frame buffer in bytes.
const int kBytesPerPixel = 4;

void CanopyEngine::SetBufferPointer(void *pixel_buffer, size_t size) {
    this->pixel_buffer = pixel_buffer;
    this->buffer_size = size;
}

void CanopyEngine::RenderFrame(unsigned long width, unsigned long height,
                               double frame_interval) {
    if (width * height * kBytesPerPixel != buffer_size) {
        // TODO: throw some kind of error.
        return;
    }

    // Track the latest requested dimenions.
    frame_width = width;
    frame_height = height;

    // Simulate.
    game_state.Update(&input_state_manager, width, height, frame_interval);

    // Render.
    // TODO: probably move to its own file.
    DrawRectWithBounds(0.0, 0.0, frame_width, frame_height, kBorderColor);
    DrawRectWithBounds(
        kBorderSize.width, kBorderSize.height, frame_width - kBorderSize.width,
        frame_height - kBorderSize.height, kArenaColor);
    DrawRectAbsolute(game_state.GetBallPosition(), kBallSize, kBallColor);
    DrawRectAbsolute(
        game_state.GetPlayerPosition(0), kPlayerSize, kPlayerColor);
    DrawRectAbsolute(
        game_state.GetPlayerPosition(1), kPlayerSize, kPlayerColor);
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
