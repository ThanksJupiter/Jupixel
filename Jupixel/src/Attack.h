#pragma once

#include "Components/ColliderComponent.h"
#include "Animation/Spritesheet.h"

#include "glm/glm.hpp"

struct Attack
{
	Attack(Spritesheet* _sheet,
		int hitbox_start_frame,
		int hitbox_stop_frame,
		glm::vec2 knockback,
		glm::vec2 hb_offset,
		glm::vec2 hb_size,
		float frame_delay,
		float damage)
	{
		Anim = _sheet;
		Hitbox_start_frame = hitbox_start_frame;
		Hitbox_stop_frame = hitbox_stop_frame;
		Damage = damage;

		Duration = frame_delay * Anim->Sprites.size();

		Knockback_direction = knockback;
		Hitbox = ColliderComponent();
		Hitbox.Offset = hb_offset;
		Hitbox.Scale = hb_size;
	}

	Spritesheet* Anim = nullptr;
	int Hitbox_start_frame = 0;
	int Hitbox_stop_frame = 0;
	float Duration = 0.0f;
	float Damage = 0.0f;

	glm::vec2 Knockback_direction = glm::vec2(1.0f, 0.5f);

	ColliderComponent Hitbox = ColliderComponent();
};
