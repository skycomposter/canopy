#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include <stdint.h>

// All colors project-wide are expressed as 32-bit unsigned ints.
typedef uint32_t Color;

// The coordinates of a point in 2D space.
typedef struct {
  double x;
  double y;
} Point2D;

// The rectangular dimensions of a 2D object.
typedef struct {
  double width;
  double height;
} Size2D;

// Absolute speed value used to calculate velocities.
typedef uint32_t Speed;

// The velocity of a 2D object.
typedef struct {
  double x;
  double y;
} Velocity2D;

#endif  // PRIMITIVES_H_
