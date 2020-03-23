#pragma once

#include "glm/glm.hpp"

struct ColliderComponent
{
	int entity_id = 0;
	bool is_active = false;
	bool is_hit = false;
	bool is_colliding = false;
	float scale = 1.0f;
	glm::vec2 Position = glm::vec2(0.0f);
};
