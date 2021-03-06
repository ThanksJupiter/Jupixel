#pragma once

#include "Key.h"
#include "MouseCodes.h"
#include <utility>

struct GamepadButtonState
{
	bool Previously_pressed = false;
};

// TODO always update gamepad state once instead of every time something is queried

void update_gamepad_state(int id);

bool is_key_pressed(KeyCode key);
bool is_mouse_button_pressed(MouseCode button);

std::pair<float, float> get_mouse_position();
float get_mouse_position_x();
float get_mouse_position_y();

std::pair<float, float> get_left_stick(int id);
float get_left_stick_x(int id);
float get_left_stick_y(int id);

std::pair<float, float> get_right_stick(int id);
float get_right_stick_x(int id);
float get_right_stick_y(int id);

float get_axis(int id, int axis);

bool is_button_held(int id, int button);
bool is_button_down(int id, int button);
