#pragma once

#include "glm/glm.hpp"

#include "Texture2D.h"
#include "Animation/Sprite.h"
#include "ColliderComponent.h"

struct Level
{
	Texture2D Texture;
	Sprite Sprite;
	ColliderComponent Collider = ColliderComponent();
	glm::vec2 Scale = glm::vec2(0.0f);
	glm::vec2 Position = glm::vec2(0.0f);
};
