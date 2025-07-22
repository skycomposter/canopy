#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

// Supported types of input events.
typedef enum {
    POINTER_MOVED
} EventType;

// The coordinates of a point in 2D space.
typedef struct {
    double x;
    double y;
} Point2D;

// An event triggered by some type of user input.
typedef struct {
    EventType type;
    Point2D position;
} InputEvent;

#endif // INPUT_EVENT_H
