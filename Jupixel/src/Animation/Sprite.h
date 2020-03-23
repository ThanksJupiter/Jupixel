#pragma once

#include "glm/glm.hpp"

struct Texture2D;

struct Sprite
{
	Sprite(Texture2D* texture, int width, int height, int index);

	Texture2D* Texture = nullptr;
	uint32_t Index = 0;
	int Width = 0;
	int Height = 0;

	glm::vec2 TopRight = glm::vec2(0.0f);
	glm::vec2 BottomRight = glm::vec2(0.0f);
	glm::vec2 BottomLeft = glm::vec2(0.0f);
	glm::vec2 TopLeft = glm::vec2(0.0f);
};
