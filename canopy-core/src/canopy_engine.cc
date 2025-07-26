#include "canopy_engine.h"

#include <algorithm>
#include <cmath>

#include "input/input.h"
#include "input/input_event.h"
#include "renderer/renderer.h"
#include "state/game_state.h"

// Size of a single pixel in the frame buffer in bytes.
const int kBytesPerPixel = 4;

void CanopyEngine::SetBufferPointer(void *pixel_buffer, size_t size) {
  renderer.SetBufferPointer(pixel_buffer, size);
}

void CanopyEngine::RenderFrame(double width, double height,
                               double delta_time) {
  if (width * height * kBytesPerPixel != renderer.GetBufferSize()) {
    // TODO: throw some kind of error.
    return;
  }

  Size2D frame_size = {width, height};

  // Simulate.
  game_state.Update(&input_state_manager, frame_size, delta_time);

  // Render.
  renderer.DrawFrame(&game_state, frame_size);
}

void CanopyEngine::OnInputEvent(InputEvent event) {
  input_state_manager.OnInputEvent(event);
}
