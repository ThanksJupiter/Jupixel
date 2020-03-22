#pragma once

#include "glm/glm.hpp"

struct Texture2D;

struct Sprite
{
	Sprite(Texture2D* _texture, int _width, int _height, int _index);

	Texture2D* texture = nullptr;
	uint32_t index = 0;
	int width = 0;
	int height = 0;

	glm::vec2 topRight = glm::vec2(0.0f);
	glm::vec2 bottomRight = glm::vec2(0.0f);
	glm::vec2 bottomLeft = glm::vec2(0.0f);
	glm::vec2 topLeft = glm::vec2(0.0f);
};
