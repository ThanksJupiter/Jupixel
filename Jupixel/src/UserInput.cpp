#include "UserInput.h"

#include "Application.h"

GamepadButtonState button_states[15];

const float deadzone = 0.25f;

bool is_key_pressed(KeyCode key)
{
	int state = glfwGetKey(get_window(), static_cast<int32_t>(key));

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool is_mouse_button_pressed(MouseCode button)
{
	int state = glfwGetMouseButton(get_window(), static_cast<int32_t>(button));

	return state == GLFW_PRESS;
}

std::pair<float, float> get_mouse_position()
{
	double xpos, ypos;
	glfwGetCursorPos(get_window(), &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}

float get_mouse_position_x()
{
	std::pair<float, float> pair = get_mouse_position();
	return pair.first;
}

float get_mouse_position_y()
{
	std::pair<float, float> pair = get_mouse_position();
	return pair.second;
}

std::pair<float, float> get_left_stick(int id)
{
	GLFWgamepadstate state;

	if (glfwGetGamepadState(GLFW_JOYSTICK_1 + id, &state))
	{
		double x, y;

		x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
		y = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];

		return { (float)x, (float) y };
	}

	return { (float)0.0f, (float)0.0f };
}

float get_left_stick_x(int id)
{
	std::pair<float, float> pair = get_left_stick(id);
	return abs(pair.first) > deadzone ? pair.first : 0;
}

float get_left_stick_y(int id)
{
	std::pair<float, float> pair = get_left_stick(id);
	return abs(pair.second) > deadzone ? pair.second : 0;
}

std::pair<float, float> get_right_stick(int id)
{
	GLFWgamepadstate state;

	if (glfwGetGamepadState(GLFW_JOYSTICK_1 + id, &state))
	{
		double x, y;

		x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
		y = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

		return { (float)x, (float)y };
	}

	return { (float)0.0f, (float)0.0f };
}

float get_right_stick_x(int id)
{
	std::pair<float, float> pair = get_right_stick(id);
	return abs(pair.first) > deadzone ? pair.first : 0;
}

float get_right_stick_y(int id)
{
	std::pair<float, float> pair = get_right_stick(id);
	return abs(pair.second) > deadzone ? pair.second : 0;
}

float get_axis(int id, int axis)
{
	GLFWgamepadstate state;

	if (glfwGetGamepadState(GLFW_JOYSTICK_1 + id, &state))
	{
		float ret_val = state.axes[axis];
		return ret_val;
	}

	return 0;
}

bool is_button_held(int id, int button)
{
	GLFWgamepadstate state;

	if (glfwGetGamepadState(GLFW_JOYSTICK_1 + id, &state))
	{
		return state.buttons[button];
	}
	
	return false;
}

bool is_button_down(int id, int button)
{
	GLFWgamepadstate state;

	if (glfwGetGamepadState(GLFW_JOYSTICK_1 + id, &state))
	{
		bool the_fuck = state.buttons[button] == GLFW_PRESS;

		if (the_fuck && !button_states[button].Previously_pressed)
		{
			button_states[button].Previously_pressed = true;
			return true;
		}
		else
		{
			button_states[button].Previously_pressed = state.buttons[button];
			return false;
		}
	}
}
