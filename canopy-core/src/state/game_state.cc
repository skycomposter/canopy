#include "state/game_state.h"

#include <cmath>
#include <cstdlib>

#include "input/input.h"
#include "input/input_codes.h"
#include "settings/settings.h"
#include "util/utils.h"

Point2D GameState::GetBallPosition() const {
  return ball_position;
}

Point2D GameState::GetPlayerPosition(unsigned int player) const {
  return player_positions[player];
}

void GameState::Update(InputStateManager *input_state_manager,
                       unsigned long frame_width, unsigned long frame_height,
                       double frame_interval) {
  // If this is the first frame, put the ball in the middle and the players in
  // their default positions.
  if (ball_position.x == -1 || ball_position.y == -1) {
    ball_position = {frame_width / 2.0, frame_height / 2.0};
  }
  if (player_positions[0].x == -1 || player_positions[0].y == -1) {
    player_positions[0] =
      {kBorderSize.width * 3 + kPlayerSize.width / 2, frame_height / 2.0};
  }
  if (player_positions[1].x == -1 || player_positions[1].y == -1) {
    player_positions[1] =
      {
        frame_width - kBorderSize.width * 3 - kPlayerSize.width / 2,
        frame_height / 2.0
      };
  }

  if ((ball_velocity.x == -1 || ball_velocity.y == -1)
      && (*input_state_manager)[kSpacebar].is_pressed) {
    // If the game has not started yet and the spacebar is pressed, give the
    // ball a random starting velocity.
    int angle = std::rand() % 360;
    ball_velocity =
      {kBallSpeed * std::cos(angle), kBallSpeed * std::sin(angle)};
  } else {
    // Update the current ball position based on velocity and time passed.
    ball_position.x += ball_velocity.x * frame_interval;
    ball_position.y += ball_velocity.y * frame_interval;
  }

  // Make sure the ball is bound by the edges of the arena.
  double horizontal_padding = kBorderSize.width + kBallSize.width / 2;
  double vertical_padding = kBorderSize.height + kBallSize.height / 2;
  ball_position.x = clamp(
    ball_position.x, horizontal_padding, frame_width - horizontal_padding);
  ball_position.y = clamp(
    ball_position.y, vertical_padding, frame_height - vertical_padding);
}
