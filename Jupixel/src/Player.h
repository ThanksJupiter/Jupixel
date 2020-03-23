#pragma once

#include "ECS/Components/InputComponent.h"
#include "Components/TransformComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/AnimationComponent.h"
#include "ECS/Components/Components.h"
#include "Components/ColliderComponent.h"
#include "Components/ActionStateComponent.h"

struct InputComponent;
struct TransformComponent;
struct PhysicsComponent;
struct AnimationComponent;
struct ColliderComponent;
struct ActionStateComponent;

struct Spritesheet;

struct Player
{
	unsigned int ID = 0;

	InputComponent* Input = new InputComponent();
	TransformComponent Transform = TransformComponent();
	PhysicsComponent Physics = PhysicsComponent();
	AnimationComponent Animation = AnimationComponent();
	ColliderComponent Collider = ColliderComponent();
	ActionStateComponent ActionState = ActionStateComponent();
};

void change_player_animation(Player* player, Spritesheet* new_anim);
void change_player_state(Player* player, int state);
void change_player_state(Player* player, int state, float duration);
