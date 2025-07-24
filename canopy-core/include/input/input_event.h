#ifndef INPUT_EVENT_H_
#define INPUT_EVENT_H_

#include "input/input_codes.h"
#include "util/primitives.h"

// Supported types of input events.
typedef enum {
    kKeyDown,
    kKeyUp
} EventType;

// An event triggered by user input.
typedef struct {
    InputCode input_code;
    EventType type;
    Point2D position;
} InputEvent;

#endif  // INPUT_EVENT_H_
