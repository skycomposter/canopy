#ifndef CANOPY_ENGINE_H_
#define CANOPY_ENGINE_H_

#include <stddef.h>

#include "input/input.h"
#include "input/input_event.h"
#include "renderer/renderer.h"
#include "state/game_state.h"

// A game engine that renders frames to a provided buffer.
class CanopyEngine {
 public:
  // Provides a valid buffer and its size to the engine.
  void SetBufferPointer(void *pixel_buffer, size_t size);

  // Processes an input event and updates the internal state accordingly.
  void OnInputEvent(InputEvent event);

  // Renders a frame from the current internal state and writes it to the
  // buffer.
  void RenderFrame(double width, double height, double delta_time);

 private:
  // Bookkeeping for the game state.
  GameState game_state;
  // Bookkeeping for user input.
  InputStateManager input_state_manager;
  // Responsible for actually drawing each frame to the buffer.
  Renderer renderer;
};

#endif  // CANOPY_ENGINE_H_
