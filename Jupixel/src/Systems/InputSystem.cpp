#include "jppch.h"

#include "InputSystem.h"

#include "UserInput.h"
#include "Key.h"
#include "Player.h"
#include "GLFW/glfw3.h"

const float c_threshold = 0.8f;
const float tilt_threshold = 0.1f;
const float smash_threshold = 0.9f;



void input_update()
{
	
}

void update_input_system(Player* player)
{
	update_gamepad_state(player->ID);

	InputComponent& input = player->Input;

	// TODO can probably make beautiful with gamepads implemented
	if (player->ID == 0)
	{
		input.Left_stick_x = is_key_pressed(KeyCode::D) ? 1.0f :
			is_key_pressed(KeyCode::A) ? -1.0f :
			get_left_stick_x(player->ID);

		input.Left_stick_y = is_key_pressed(KeyCode::W) ? 1.0f :
			is_key_pressed(KeyCode::S) ? -1.0f :
			-get_left_stick_y(player->ID);

		input.Jump = is_key_pressed(KeyCode::Q) ||
			is_button_down(player->ID, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER) ||
			is_button_down(player->ID, GLFW_GAMEPAD_BUTTON_Y);
		input.Attack = is_key_pressed(KeyCode::E) || is_button_down(player->ID, GLFW_GAMEPAD_BUTTON_A);
		input.Block = is_key_pressed(KeyCode::R) || get_axis(player->ID, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) != -1.0f;
	}
	else
	{
		input.Left_stick_x = is_key_pressed(KeyCode::L) ? 1.0f :
			is_key_pressed(KeyCode::J) ? -1.0f :
			get_left_stick_x(player->ID);

		input.Left_stick_y = is_key_pressed(KeyCode::I) ? 1.0f :
			is_key_pressed(KeyCode::K) ? -1.0f :
			-get_left_stick_y(player->ID);

		input.Jump = is_key_pressed(KeyCode::U) || 
			is_button_down(player->ID, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER) ||
			is_button_down(player->ID, GLFW_GAMEPAD_BUTTON_Y);
		input.Attack = is_key_pressed(KeyCode::O) || is_button_down(player->ID, GLFW_GAMEPAD_BUTTON_A);
		float axis = get_axis(player->ID, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER);
		// HACK is 0 when no controller?
		input.Block = is_key_pressed(KeyCode::Y) || axis != -0.0f;
	}

	input.Jump_held = is_button_held(player->ID, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER) ||
		is_button_held(player->ID, GLFW_GAMEPAD_BUTTON_Y);

	float convert_axis_r = get_axis(player->ID, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) * 0.5f;
	input.Right_trigger = convert_axis_r + 0.5f;

	float convert_axis_l = get_axis(player->ID, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) * 0.5f;
	input.Left_trigger = convert_axis_l + 0.5f;

	float rs_x = get_right_stick_x(player->ID);
	float rs_y = get_right_stick_y(player->ID);

	float ls_x = input.Left_stick_x;
	float ls_y = input.Left_stick_y;

	if (input.Attack)
	{
		// smash
		if (ls_x > smash_threshold)
		{
			input.Smash_right = true;
		}
		else if (ls_x < -smash_threshold)
		{
			input.Smash_left = true;
		}
		else
		{
			input.Smash_right = false;
			input.Smash_left = false;
		}

		if (ls_y > smash_threshold)
		{
			input.Smash_down = true;
		}
		else if (ls_y < -smash_threshold)
		{
			input.Smash_up = true;
		}
		else
		{
			input.Smash_down = false;
			input.Smash_up = false;
		}

		// tilt
		if (ls_x > tilt_threshold)
		{
			input.Tilt_right = true;
		} 
		else if (ls_x < -tilt_threshold)
		{
			input.Tilt_left = true;
		}
		else
		{
			input.Tilt_right = false;
			input.Tilt_left = false;
		}

		if (ls_y > tilt_threshold)
		{
			input.Tilt_up = true;
		} 
		else if (ls_y < -tilt_threshold)
		{
			input.Tilt_down = true;
		}
		else
		{
			input.Tilt_down = false;
			input.Tilt_up = false;
		}
	}
	else
	{
		input.Smash_down = false;
		input.Smash_up = false;
		input.Smash_right = false;
		input.Smash_left = false;

		input.Tilt_down = false;
		input.Tilt_up = false;
		input.Tilt_right = false;
		input.Tilt_left = false;
	}

	input.Smash_up = rs_y < -smash_threshold;
	input.Smash_down = rs_y > smash_threshold;
	input.Smash_right = rs_x > smash_threshold;
	input.Smash_left = rs_x < -smash_threshold;

	if (rs_x < -1.0f || rs_y < -1.0f)
	{
		return;
	}

	input.C_down = rs_y > c_threshold;
	input.C_up = rs_y < -c_threshold;
	input.C_right = rs_x > c_threshold;
	input.C_left = rs_x < -c_threshold;
}
