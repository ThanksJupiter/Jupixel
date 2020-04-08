#pragma once

#include "glm/glm.hpp"

#include "Components/AnimationComponent.h"
#include "../Components/VFXComponent.h"
#include "../Animation/Spritesheet.h"

struct TransformComponent;
struct PhysicsComponent;

void vfx_spawn_effect(
	Spritesheet* sheet,
	glm::vec2 position,
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
	TransformComponent* transform = nullptr,
	PhysicsComponent* physics = nullptr,
	AnimationState state = AnimationState::LastFrameStick,
	int repeat_cycles = 2);

void vfx_update(float dt);
void vfx_update_effect(VFXComponent& effect, float dt);

void vfx_update_effect_data(VFXComponent& effect, Sprite* new_sprite);
