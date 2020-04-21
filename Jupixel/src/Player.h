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
#include "Components/MatchDataComponent.h"
#include "Components/StateComponent.h"

struct Spritesheet;
struct NetworkComponent;

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
	MatchDataComponent MatchData = MatchDataComponent();
	StateComponent StateC = StateComponent();
	NetworkComponent* Network = nullptr;

	void set_position_state(State state);
	void set_action_state(State state);
	void set_locomotion_state(State state);

	void perform_grounded_attack(Attack attack);
	void perform_aerial_attack(Attack attack);

	bool is_facing_travel_direction();
	bool is_inputting_in_travel_direction();
	bool is_grounded();
};

void setup_player(Player* player_to_setup, Player* player_opponent, int player_id);
void change_player_animation(Player* player, Spritesheet* new_anim);
void change_player_animation(Player* player, Spritesheet* new_anim, AnimationState state);
void set_player_state(Player* player, PositionState state);
void set_player_state(Player* player, ActionState state);
