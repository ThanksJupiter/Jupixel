#pragma once

#include "Key.h"
#include "MouseCodes.h"
#include <utility>

bool is_key_pressed(KeyCode key);
bool is_mouse_button_pressed(MouseCode button);

std::pair<float, float> get_mouse_position();
float get_mouse_position_x();
float get_mouse_position_y();
