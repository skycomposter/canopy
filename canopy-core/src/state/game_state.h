#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include "input/input.h"
#include "util/primitives.h"

// A container for the current state of the game.
class GameState {
 public:
  Point2D GetBallPosition() const;
  Point2D GetPlayerPosition(unsigned int player) const;

  // Updates the state of the game based on current input state, frame
  // dimensions, and frame interval.
  void Update(InputStateManager *input_state_manager,
              unsigned long frame_width, unsigned long frame_height,
              double frame_interval);

 private:
  Point2D ball_position = {-1, -1};
  Velocity2D ball_velocity = {-1, -1};
  Point2D player_positions[2] = {{-1, -1}, {-1, -1}};
};

#endif  // GAME_STATE_H_
