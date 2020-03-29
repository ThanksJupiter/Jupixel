#pragma once

#include "Components/AnimationComponent.h"

struct Player;

void update_position_system(Player* player, float dt);
void update_position_system(Player* player_one, Player* player_two, float dt);

void grounded_physics_update(Player* player, float dt);
void airborne_physics_update(Player* player, float dt);

void physics_idle_update(Player* player, float dt);
void physics_walk_update(Player* player, float dt);
void physics_attack_update(Player* player, float dt);
void physics_jump_update(Player* player, float dt);
void physics_fall_update(Player* player, float dt);

void physics_flip_on_input(float x_input, AnimationComponent& anim);
void physics_set_velocity_to_input(float x_input, float& x_velocity);
void physics_apply_velocity_to_position(Player* player, float dt);
void physics_apply_drag(Player* player, float dt);

void physics_land_on_touch_ground(Player* player);
