#pragma once

#include "InputComponent.h"
#include "glm/glm.hpp"

struct PositionComponent
{
	int entity_id = 0;
	float x = 0;
	float y = 0;
};

struct VelocityComponent
{
	int entity_id = 0;
	float x = 0;
	float y = 0;
};

struct RenderComponent
{
	int entity_id = 0;
	float x = 0;
	float y = 0;
	glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec2 Scale = glm::vec2(1.0f);
	struct Texture2D* texture = nullptr;
	int currentSpriteIndex = 0;
	float currentSpriteTime = 0.0f;
	float nextSpriteDelay = 0.1f;
};

struct ColliderComponent
{
	int entity_id = 0;
	bool is_active = false;
	bool is_hit = false;
	bool is_colliding = false;
	float scale = 1.0f;
	glm::vec2 offset = glm::vec2(0.0f);
};
