#include "input.h"

#include <vector>

#include "input/input_codes.h"
#include "input/input_event.h"
#include "util/primitives.h"

InputState InputStateManager::operator[](InputCode input_code) {
  states_mutex.lock();
  // Return a copy so the stored value can't get messed with.
  InputState state = states[input_code];
  states_mutex.unlock();
  return state;
}

void InputStateManager::OnInputEvent(InputEvent event) {
  states_mutex.lock();

  switch (event.type) {
    case kKeyDown:
    case kKeyUp: {
      InputState *state = &states[event.input_code];
      if (state == nullptr) {
        // TODO: handle error for missing key somehow.
        return;
      }

      bool was_pressed = state->is_pressed;
      state->is_pressed = event.type == kKeyDown;
      state->has_changed = state->is_pressed != was_pressed;

      break;
    }
  }

  states_mutex.unlock();
}

void InputStateManager::InitStates() {
  states_mutex.lock();

  for (InputCode code : input_codes) {
    InputState state;
    state.is_pressed = false;
    state.has_changed = false;
    states[code] = state;
  }

  states_mutex.unlock();
}
