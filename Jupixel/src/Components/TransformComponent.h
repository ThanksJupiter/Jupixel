#pragma once

#include "glm/glm.hpp"

struct TransformComponent
{
	glm::vec2 Position = glm::vec2(0.0f);
	glm::vec2 Scale = glm::vec2(0.3f, 0.6f);
};
