#pragma once

#include "glm/glm.hpp"

struct Spritesheet;

struct AttacksComponent
{
	glm::vec2 Hitbox_offset = glm::vec2(0.5f, 0.0f);
	glm::vec2 Hitbox_scale = glm::vec2(0.5f);
	float Duration = 0.5f;
	Spritesheet* Attack_sheet = nullptr;
	int Attack_frame_index = 4;
};
