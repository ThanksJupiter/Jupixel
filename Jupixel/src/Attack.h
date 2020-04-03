#pragma once

#include "Components/ColliderComponent.h"
#include "Animation/Spritesheet.h"

#include "glm/glm.hpp"

struct Attack
{
	Attack(Spritesheet* _sheet,
		int _hitbox_frame,
		glm::vec2 _knockback,
		glm::vec2 _hb_offset,
		glm::vec2 _hb_size,
		float frame_delay,
		float damage)
	{
		Anim = _sheet;
		Hitbox_frame = _hitbox_frame;
		Damage = damage;

		Duration = frame_delay * Anim->Sprites.size();

		Knockback_direction = _knockback;
		Hitbox = ColliderComponent();
		Hitbox.Offset = _hb_offset;
		Hitbox.Scale = _hb_size;
	}

	Spritesheet* Anim = nullptr;
	int Hitbox_frame = 0;
	float Duration = 0.0f;
	float Damage = 0.0f;

	glm::vec2 Knockback_direction = glm::vec2(1.0f, 0.5f);

	ColliderComponent Hitbox = ColliderComponent();
};
