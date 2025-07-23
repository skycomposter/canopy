#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

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

#endif  // PRIMITIVES_H_
