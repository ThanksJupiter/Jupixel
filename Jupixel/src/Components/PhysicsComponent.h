#pragma once

#include "glm/glm.hpp"

struct PhysicsComponent
{
	glm::vec2 Acceleration = glm::vec2(0.0f);
	glm::vec2 Velocity = glm::vec2(0.0f);
};
