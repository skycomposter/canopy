#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "util/primitives.h"

// Colors
const Color kArenaColor = 0xFF000000;
const Color kBallColor = 0xFF00FF00;
const Color kBorderColor = 0xFF00FF00;
const Color kPlayerColor = 0xFF00FF00;

// Arena settings
const Size2D kBorderSize = {30, 30};

// Ball settings.
const Size2D kBallSize = {32, 32};
const Speed kBallSpeed = 800;

// Player settings.
const Size2D kPlayerSize = {32, 128};
const Speed kPlayerSpeed = 800;

#endif  // SETTINGS_H_
