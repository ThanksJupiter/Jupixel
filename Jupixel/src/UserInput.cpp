#include "jppch.h"

#include "UserInput.h"

#include "Application.h"

GLFWgamepadstate state[4];
GLFWgamepadstate previous_state[4];

const float deadzone = 0.25f;

void update_gamepad_state(int id)
{
	previous_state[id] = state[id];
	glfwGetGamepadState(GLFW_JOYSTICK_1 + id, &state[id]);
}

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
	return { (float)state[id].axes[GLFW_GAMEPAD_AXIS_LEFT_X],
			 (float)state[id].axes[GLFW_GAMEPAD_AXIS_LEFT_Y] };
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
	return { (float)state[id].axes[GLFW_GAMEPAD_AXIS_RIGHT_X],
			 (float)state[id].axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] };
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
	return state[id].axes[axis];
}

bool is_button_held(int id, int button)
{
	return state[id].buttons[button];
}

bool is_button_down(int id, int button)
{
	return state[id].buttons[button]&& !previous_state[id].buttons[button];
}
