#pragma once

#include "glm/glm.hpp"

struct Spritesheet;

struct AnimationComponent
{
	Spritesheet* Current_anim = nullptr;
	glm::vec4 Color = glm::vec4(1.0f);
	glm::vec2 Animation_scale = glm::vec2(1.0f);
	bool Is_flipped = false;
	int Current_Sprite_Index = 0;
	float Current_sprite_time = 0.0f;
	float Next_sprite_delay = 0.1f;
};
