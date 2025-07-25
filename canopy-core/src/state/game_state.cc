#include "state/game_state.h"

#include "input/input.h"
#include "input/input_codes.h"
#include "settings/settings.h"
#include "util/utils.h"

Point2D GameState::GetBallPosition() const {
    return ball_position;
}

void GameState::Update(InputStateManager *input_state_manager,
                       unsigned long frame_width, unsigned long frame_height,
                       double frame_interval) {
       // If this is the first frame, put the ball in the middle.
    if (ball_position.x == -1 || ball_position.y == -1) {
        ball_position = {frame_width / 2.0, frame_height / 2.0};
    }

    // Update the current ball position based on the latest input.
    if ((*input_state_manager)[kLeftArrow].is_pressed) {
        ball_position.x -= kBallVelocity.x * frame_interval; 
    }
    if ((*input_state_manager)[kUpArrow].is_pressed) {
        ball_position.y -= kBallVelocity.y * frame_interval; 
    }
    if ((*input_state_manager)[kRightArrow].is_pressed) {
        ball_position.x += kBallVelocity.x * frame_interval; 
    }
    if ((*input_state_manager)[kDownArrow].is_pressed) {
        ball_position.y += kBallVelocity.y * frame_interval; 
    }

    // Make sure the ball is bound by the edges of the arena.
    double horizontal_padding = kBorderSize.width + kBallSize.width / 2;
    double vertical_padding = kBorderSize.height + kBallSize.height / 2;
    ball_position.x = clamp(ball_position.x, horizontal_padding,
        frame_width - horizontal_padding);
    ball_position.y = clamp(ball_position.y, vertical_padding,
        frame_height - vertical_padding);
}
