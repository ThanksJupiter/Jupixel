#pragma once

#include "Input.h"
#include "glm/glm.hpp"

struct Position
{
	int entity_id = 0;
	float x = 0;
	float y = 0;
};

struct Velocity
{
	int entity_id = 0;
	float x = 0;
	float y = 0;
};

struct Render
{
	int entity_id = 0;
	int x = 0;
	int y = 0;
	glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};
