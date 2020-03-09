#include "UserInput.h"

#include "Application.h"

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
