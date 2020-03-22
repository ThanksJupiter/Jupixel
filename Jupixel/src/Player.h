#pragma once

struct InputComponent;
struct TransformComponent;
struct PhysicsComponent;
struct AnimationComponent;

struct Player
{
	unsigned int ID = 0;

	InputComponent* input = nullptr;
	TransformComponent* transform = nullptr;
	PhysicsComponent* physics = nullptr;
	AnimationComponent* animation = nullptr;
};
