#pragma once

#include "glm/glm.hpp"
#include "Components/ColliderComponent.h"
#include <vector>

struct RaycastHit
{
	glm::vec2 point = glm::vec2(0.0f);
	glm::vec2 normal = glm::vec2(0.0f);
};

bool raycast(glm::vec2 from_position, glm::vec2 direction, float distance, RaycastHit& hit);
void set_target_colliders(std::vector<ColliderComponent*> colliders);
void add_target_collider(ColliderComponent* collider);
