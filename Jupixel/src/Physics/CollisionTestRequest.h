#pragma once

#include "glm/glm.hpp"
#include "Components/ColliderComponent.h"

struct Player;

struct CollisionTestRequest
{
	Player* Instigator = nullptr;
	Player* Target = nullptr;

	ColliderComponent Collider = ColliderComponent();

	bool Is_resolved = false;
};
