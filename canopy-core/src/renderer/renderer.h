#ifndef RENDERER_H_
#define RENDERER_H_

#include <stddef.h>

#include "state/game_state.h"
#include "util/primitives.h"

// The class in charge of drawing each frame to the buffer.
class Renderer {
 public:
  void SetBufferPointer(void *pixel_buffer, size_t size);
  size_t GetBufferSize();

  // Use the current game state to draw a frame to the buffer.
  void DrawFrame(GameState *game_state, Size2D frame_size);

 private:
  // Shared frame buffer in which frames are rendered.
  void *pixel_buffer = nullptr;
  // Allocated size of the pixel_buffer.
  size_t buffer_size = 0;

  // Draws a rectangle with the given center position, dimensions, and color.
  void DrawRectAbsolute(Point2D position, Size2D size, Size2D frame_size,
                        Color color);
  // Draws a rectangle with the given center position and dimensions (expressed
  // as percentages of the frame dimensions), and color of the frame.
  void DrawRectRelative(Point2D position, Size2D size, Size2D frame_size,
                        Color color);
  // Draws a rectangle with the given bounds and color.
  void DrawRectWithBounds(int left, int top, int right, int bottom,
                          Size2D frame_size, Color color);
};

#endif  // RENDERER_H_
