#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include "input/input.h"
#include "settings/settings.h"
#include "util/primitives.h"

// A container for the current state of the game.
class GameState {
 public:
  Point2D GetBallPosition() const;
  Point2D GetPlayerPosition(unsigned int player) const;

  // Updates the state of the game based on current input state, frame
  // dimensions, and delta time.
  void Update(InputStateManager *input_state_manager, Size2D frame_size,
              double delta_time);

 private:
  Point2D ball_position = {-1, -1};
  Velocity2D ball_velocity = {0, 0};
  Point2D player_positions[kPlayerCount] = {{-1, -1}, {-1, -1}};
  Velocity2D player_velocities[kPlayerCount] = {{0, 0}, {0, 0}};

  // Initializes the positions of the ball and players based on the frame size.
  // If they have already been initialized, this method does nothing.
  void MaybeInitState(Size2D frame_size);

  // If the ball is moving, updates its position. If not, give it a random
  // initial direction and kicks off its movement when the spacebar is pressed.
  void UpdateBallState(InputStateManager *input_state_manager,
                       Size2D frame_size, double delta_time);
  // Updates the position and velocity of a player based on user input.
  void UpdatePlayerState(int player_index,
                         InputStateManager *input_state_manager,
                         Size2D frame_size, double delta_time);
};

#endif  // GAME_STATE_H_
