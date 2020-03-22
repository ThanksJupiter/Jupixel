#pragma once

#include "glm/glm.hpp"

struct Spritesheet;

struct AnimationComponent
{
	Spritesheet* current_anim = nullptr;
	glm::vec4 Color = glm::vec4(1.0f);
	bool isFlipped = false;
	int currentSpriteIndex = 0;
	float currentSpriteTime = 0.0f;
	float nextSpriteDelay = 0.1f;
};
