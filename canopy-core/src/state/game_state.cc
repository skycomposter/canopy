#include "state/game_state.h"

#include <cmath>
#include <cstdlib>
#include <ctime>

#include "input/input.h"
#include "input/input_codes.h"
#include "settings/settings.h"
#include "util/utils.h"

// TODO: could probably put controls into a better struct, or maybe players
//   altogether.

// Inputs used to move each of the players up and down.
const InputCode player_controls[kPlayerCount][2] = {
  {kWKey, kSKey},
  {kUpArrow, kDownArrow},
};

Point2D GameState::GetBallPosition() const {
  return ball_position;
}

Point2D GameState::GetPlayerPosition(unsigned int player) const {
  return player_positions[player];
}

void GameState::Update(InputStateManager *input_state_manager,
                       Size2D frame_size, double delta_time) {
  MaybeInitState(frame_size);
  UpdateBallState(input_state_manager, frame_size, delta_time);
  UpdatePlayerState(0, input_state_manager, frame_size, delta_time);
  UpdatePlayerState(1, input_state_manager, frame_size, delta_time);
}

void GameState::MaybeInitState(Size2D frame_size) {
  // If the ball has a valid position, the state was already initialized.
  if (ball_position.x != -1 && ball_position.y != -1) { return; }

  double frame_half_width = frame_size.width / 2;
  double frame_half_height = frame_size.height / 2;
  double player_half_width = kPlayerSize.width / 2;

  // Place the ball in the middle of the arena.
  ball_position = {frame_half_width, frame_half_height};

  // Place the players at opposite ends of the arena.
  player_positions[0] =
    {kBorderSize.width * 3 + player_half_width, frame_half_height};
  player_positions[1] =
    {frame_size.width - kBorderSize.width * 3 - player_half_width,
      frame_half_height};
}

void GameState::UpdateBallState(InputStateManager *input_state_manager,
                                Size2D frame_size, double delta_time) {
  if (ball_velocity.x == 0 || ball_velocity.y == 0) {
    if (!(*input_state_manager)[kSpacebar].is_pressed) {
      // If the game has not started yet and the spacebar is now pressed,
      // there is nothing to do.
      return;
    }

    // If the spacebar is pressed, give the ball a random starting velocity.
    std::srand(std::time(0));
    int angle = std::rand() % 400; // radians
    ball_velocity =
      {kBallSpeed * std::cos(angle), kBallSpeed * std::sin(angle)};
  } else {
    double ball_half_width = kBallSize.width / 2;
    double ball_half_height = kBallSize.height / 2;

    ball_position.x += ball_velocity.x * delta_time;
    ball_position.y += ball_velocity.y * delta_time;

    double horizontal_padding = kBorderSize.width + ball_half_width;
    double vertical_padding = kBorderSize.height + ball_half_height;

    // Detect wall collisions and bounce.
    if (ball_position.x < horizontal_padding ||
        ball_position.x > frame_size.width - horizontal_padding) {
      ball_velocity.x = -ball_velocity.x;
    }
    if (ball_position.y < vertical_padding ||
        ball_position.y > frame_size.height - vertical_padding) {
      ball_velocity.y = -ball_velocity.y;
    }

    // Make sure the ball is bound by the edges of the arena.
    ball_position.x = clamp(
      ball_position.x,
      horizontal_padding,
      frame_size.width - horizontal_padding);
    ball_position.y = clamp(
      ball_position.y, vertical_padding, frame_size.height - vertical_padding);
  }
}

void GameState::UpdatePlayerState(int player_index,
                                  InputStateManager *input_state_manager,
                                  Size2D frame_size, double delta_time) {
  Point2D *position = &player_positions[player_index];
  Velocity2D *velocity = &player_velocities[player_index];
  double acceleration = player_velocities[player_index].y * -5;
  const InputCode *controls = player_controls[player_index];

  if ((*input_state_manager)[controls[0]].is_pressed) {
    acceleration -= 8000;
  } else if ((*input_state_manager)[controls[1]].is_pressed) {
    acceleration += 8000;
  }

  position->y += (velocity->y * delta_time) +
    (acceleration * delta_time * delta_time) / 2;
  velocity->y += acceleration * delta_time;

  // Detect wall collisions and stop moving.
  double padding = kBorderSize.height + kPlayerSize.height / 2;
  if (position->y < padding || position->y > frame_size.height - padding) {
    position->y = clamp(
      position->y,
      padding,
      frame_size.height - padding);
    velocity->y = 0;
  }
}
