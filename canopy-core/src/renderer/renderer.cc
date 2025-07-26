#include "renderer/renderer.h"

#include <stddef.h>

#include "state/game_state.h"
#include "settings/settings.h"
#include "util/primitives.h"

void Renderer::SetBufferPointer(void *pixel_buffer, size_t size) {
  this->pixel_buffer = pixel_buffer;
  this->buffer_size = size;
}

size_t Renderer::GetBufferSize() {
  return buffer_size;
}

void Renderer::DrawFrame(GameState *game_state, Size2D frame_size) {
  // Background.
  DrawRectWithBounds(
    0.0, 0.0, frame_size.width, frame_size.height, frame_size, kBorderColor);

  // Arena.
  DrawRectWithBounds(
    kBorderSize.width,
    kBorderSize.height,
    frame_size.width - kBorderSize.width,
    frame_size.height - kBorderSize.height,
    frame_size,
    kArenaColor);

  // Ball.
  DrawRectAbsolute(
    game_state->GetBallPosition(), kBallSize, frame_size, kBallColor);

  // Players.
  DrawRectAbsolute(
    game_state->GetPlayerPosition(0), kPlayerSize, frame_size, kPlayerColor);
  DrawRectAbsolute(
    game_state->GetPlayerPosition(1), kPlayerSize, frame_size, kPlayerColor);
}

void Renderer::DrawRectAbsolute(Point2D position, Size2D size,
                                Size2D frame_size, Color color) {
  double half_width = size.width / 2;
  double half_height = size.height / 2;
  int left = position.x - half_width;
  int top = position.y - half_height;
  int right = position.x + half_width;
  int bottom = position.y + half_height;
  DrawRectWithBounds(left, top, right, bottom, frame_size, color);
}

void Renderer::DrawRectRelative(Point2D position, Size2D size,
                                Size2D frame_size, Color color) {
  double half_width = size.width / 2;
  double half_height = size.height / 2;
  int left = (position.x - half_width) * frame_size.width;
  int top = (position.y - half_height) * frame_size.height;
  int right = (position.x + half_width) * frame_size.width;
  int bottom = (position.y + half_height) * frame_size.height;
  DrawRectWithBounds(left, top, right, bottom, frame_size, color);
}

void Renderer::DrawRectWithBounds(int left, int top, int right, int bottom,
                                  Size2D frame_size, Color color) {
  for (int y = top; y < bottom; y++) {
    unsigned int *pixel = static_cast<unsigned int *>(this->pixel_buffer) +
      left + y * (int) frame_size.width;
    for (int x = left; x < right; x++) {
      *pixel++ = color;
    }
  }
}
