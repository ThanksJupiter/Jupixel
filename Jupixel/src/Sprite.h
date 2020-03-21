#pragma once

#include "glm/glm.hpp"

struct Texture2D;

struct Sprite
{
	Sprite(Texture2D* texture, int size);

	Texture2D* texture;

	// maybe
	uint32_t index;

	glm::vec2 topRight = glm::vec2(0.0f);
	glm::vec2 bottomRight = glm::vec2(0.0f);
	glm::vec2 bottomLeft = glm::vec2(0.0f);
	glm::vec2 topLeft = glm::vec2(0.0f);
};
