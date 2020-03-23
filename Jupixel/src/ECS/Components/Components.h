#pragma once

#include "InputComponent.h"
#include "glm/glm.hpp"

struct Spritesheet;
struct Texture2D;

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
	bool isMoving = false;
};

struct RenderComponent
{
	int entity_id = 0;
	float x = 0;
	float y = 0;
	glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec2 Scale = glm::vec2(1.0f);
	Texture2D* texture = nullptr;
	Spritesheet* current_anim_sheet = nullptr;
	bool isFlipped = false;
	int currentSpriteIndex = 0;
	float currentSpriteTime = 0.0f;
	float nextSpriteDelay = 0.1f;
};
