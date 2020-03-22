#pragma once
#include "ECS/Components/InputComponent.h"
#include "Components/TransformComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/AnimationComponent.h"
#include "ECS/Components/Components.h"

struct InputComponent;
struct TransformComponent;
struct PhysicsComponent;
struct AnimationComponent;
struct ColliderComponent;

struct Player
{
	unsigned int ID = 0;

	InputComponent* input = new InputComponent();
	TransformComponent transform = TransformComponent();
	PhysicsComponent physics = PhysicsComponent();
	AnimationComponent animation = AnimationComponent();
	ColliderComponent collider = ColliderComponent();
};
