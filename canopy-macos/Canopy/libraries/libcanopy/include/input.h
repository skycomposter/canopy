#ifndef INPUT_H
#define INPUT_H

#include <mutex>
#include <unordered_map>

#include "input_codes.h"
#include "input_event.h"
#include "util/primitives.h"

// Current state of a single input (mouse, keyboard key, or controller button).
typedef struct {
    // Whether the input is currently pressed (where applicable).
    bool is_pressed;
    // Current position on the window (where applicable).
    Point2D position;
    // Whether the pressed state or position have changed since the last
    // frame (where applicable).
    bool has_changed;
} InputState;

// A manager for all bookkeeping and handling of user input.
class InputStateManager {
public:
    InputStateManager() {
        InitStates();
    }

    // Fetches a single input state with concurrency protections. The returned
    // value is a copy, so this operator cannot be used as a setter.
    InputState operator[](InputCode input_code);

    // Processes a user input event and updates the internal state accordingly.
    void OnInputEvent(InputEvent event);

private:
    // Source of truth for the latest state of user input.
    std::unordered_map<InputCode, InputState> states;
    // Guards input_states.
    std::mutex states_mutex;

    // Initialize the internal state with default values.
    void InitStates();
};

#endif // INPUT_H
