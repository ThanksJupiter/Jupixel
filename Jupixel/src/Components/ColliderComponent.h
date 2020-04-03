#pragma once

#include "glm/glm.hpp"

struct ColliderComponent
{
	int entity_id = 0;
	bool Is_active = false;
	bool Is_hit = false;
	bool Is_colliding = false;
	bool Flip = false;
	glm::vec2 Pending_knockback = glm::vec2(0.0f);
	float Pending_damage = 0.0f;
	glm::vec2 Scale = glm::vec2(1.0f, 0.5f);
	glm::vec2 Position = glm::vec2(0.0f);
	glm::vec2 Offset = glm::vec2(0.0f);

	glm::vec2 Min() { return glm::vec2(Position.x - Scale.x, Position.y - Scale.y); }
	glm::vec2 Max() { return glm::vec2(Position.x + Scale.x, Position.y + Scale.y); }
};
