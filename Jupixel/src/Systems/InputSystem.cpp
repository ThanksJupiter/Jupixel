#include "InputSystem.h"

#include "UserInput.h"
#include "Key.h"
#include "Player.h"
#include "GLFW/glfw3.h"

const float c_threshold = 0.8f;

void update_input_system(Player* player)
{
	InputComponent* input = player->Input;

	// TODO can probably make beautiful with gamepads implemented
	if (player->ID == 0)
	{
		input->Left_stick_x = is_key_pressed(KeyCode::D) ? 1.0f :
			is_key_pressed(KeyCode::A) ? -1.0f :
			get_left_stick_x(player->ID);

		input->Left_stick_y = is_key_pressed(KeyCode::W) ? 1.0f :
			is_key_pressed(KeyCode::S) ? -1.0f :
			get_left_stick_y(player->ID);

		input->Jump = is_key_pressed(KeyCode::Q) || is_button_pressed(player->ID, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER);
		input->Attack = is_key_pressed(KeyCode::E) || is_button_pressed(player->ID, GLFW_GAMEPAD_BUTTON_X);
	}
	else
	{
		input->Left_stick_x = is_key_pressed(KeyCode::L) ? 1.0f :
			is_key_pressed(KeyCode::J) ? -1.0f :
			get_left_stick_x(player->ID);

		input->Left_stick_y = is_key_pressed(KeyCode::I) ? 1.0f :
			is_key_pressed(KeyCode::K) ? -1.0f :
			get_left_stick_y(player->ID);

		input->Jump = is_key_pressed(KeyCode::U) || is_button_pressed(player->ID, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER);
		input->Attack = is_key_pressed(KeyCode::O) || is_button_pressed(player->ID, GLFW_GAMEPAD_BUTTON_X);
	}

	float rs_x = get_right_stick_x(player->ID);
	float rs_y = get_right_stick_y(player->ID);

	if (rs_x < -1.0f || rs_y < -1.0f)
	{
		return;
	}

	input->C_down = rs_y > c_threshold;
	input->C_up = rs_y < -c_threshold;
	input->C_right = rs_x > c_threshold;
	input->C_left = rs_x < -c_threshold;
}
