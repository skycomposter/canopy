#include "state/game_state.h"

#include <cmath>
#include <cstdlib>
#include <ctime>

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
                       Size2D frame_size, double delta_time) {
  double frame_half_width = frame_size.width / 2;
  double frame_half_height = frame_size.height / 2;
  double ball_half_width = kBallSize.width / 2;
  double ball_half_height = kBallSize.width / 2;
  double player_half_width = kPlayerSize.width / 2;

  // If this is the first frame, put the ball in the middle and the players in
  // their default positions.
  if (ball_position.x == -1 || ball_position.y == -1) {
    ball_position = {frame_half_width, frame_half_height};
  }
  if (player_positions[0].x == -1 || player_positions[0].y == -1) {
    player_positions[0] =
      {kBorderSize.width * 3 + player_half_width, frame_half_height};
  }
  if (player_positions[1].x == -1 || player_positions[1].y == -1) {
    player_positions[1] =
      {frame_size.width - kBorderSize.width * 3 - player_half_width,
       frame_half_height};
  }

  if ((ball_velocity.x == -1 || ball_velocity.y == -1)
      && (*input_state_manager)[kSpacebar].is_pressed) {
    // If the game has not started yet and the spacebar is pressed, give the
    // ball a random starting velocity.
    std::srand(std::time(0));
    int angle = std::rand() % 400; // radians
    ball_velocity =
      {kBallSpeed * std::cos(angle), kBallSpeed * std::sin(angle)};
  } else {
    // Update the current ball position based on velocity and time passed.
    ball_position.x += ball_velocity.x * delta_time;
    ball_position.y += ball_velocity.y * delta_time;

    // Detect wall collisions and bounce.
    if (ball_position.x - ball_half_width < kBorderSize.width ||
        ball_position.x + ball_half_width >
          frame_size.width - kBorderSize.width) {
      ball_velocity.x = -ball_velocity.x;
    }
    if (ball_position.y - ball_half_height < kBorderSize.height ||
        ball_position.y + ball_half_height >
          frame_size.height - kBorderSize.height) {
      ball_velocity.y = -ball_velocity.y;
    }
  }

  // Make sure the ball is bound by the edges of the arena.
  double horizontal_padding = kBorderSize.width + ball_half_width;
  double vertical_padding = kBorderSize.height + ball_half_height;
  ball_position.x = clamp(
    ball_position.x,
    horizontal_padding,
    frame_size.width - horizontal_padding);
  ball_position.y = clamp(
    ball_position.y, vertical_padding, frame_size.height - vertical_padding);
}
