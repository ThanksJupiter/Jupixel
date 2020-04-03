#pragma once

#include "Components/InputComponent.h"
#include "Components/TransformComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/Components.h"
#include "Components/ColliderComponent.h"
#include "Components/ActionStateComponent.h"
#include "Components/CombatComponent.h"
#include "Components/LocomotionComponent.h"

struct Spritesheet;

struct Player
{
	unsigned int ID = 0;

	Player* Opponent = nullptr;
	bool Reset_time_scale_on_land = false;

	InputComponent Input = InputComponent();
	TransformComponent Transform = TransformComponent();
	PhysicsComponent Physics = PhysicsComponent();
	AnimationComponent Animation = AnimationComponent();
	ColliderComponent Collider = ColliderComponent();
	ActionStateComponent ActionState = ActionStateComponent();
	CombatComponent Combat = CombatComponent();
	LocomotionComponent Locomotion = LocomotionComponent();
};

void setup_player(Player* player_to_setup, Player* player_opponent, int player_id);
void change_player_animation(Player* player, Spritesheet* new_anim);
void change_player_animation(Player* player, Spritesheet* new_anim, AnimationState state);
void set_player_state(Player* player, PositionState state);
void set_player_state(Player* player, ActionState state);
