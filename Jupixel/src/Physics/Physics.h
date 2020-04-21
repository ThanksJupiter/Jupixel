#pragma once

#include "Player.h"

void phys_flip_on_input(Player& player, float dt);
void phys_set_velocity_to_input(float x_input, float& x_velocity);
void phys_add_input_to_velocity(float x_input, float& x_velocity, float dt);
void phys_apply_input_to_velocity(Player& player, float dt);
void phys_apply_drag(Player& player, float dt);
void phys_check_grab_ledge(Player& player, float dt);
void phys_land_on_touch_ground(Player& player);
