#pragma once

#include "glm/glm.hpp"

#include "Renderer/Renderer.h"
#include "AnimationComponent.h"

struct TransformComponent;
struct PhysicsComponent;

struct VFXComponent
{
	VFXComponent() {}

	RenderData Data;
	glm::vec2 Position = glm::vec2(0.0f);
	TransformComponent* Transform = nullptr;
	PhysicsComponent* physics = nullptr;
	
	glm::vec2 Pending_VFX_position = glm::vec2(0.0f);

	AnimationComponent Anim;

	bool Is_active = false;
	int Repeat_count = 0;
	int Current_repeats = 0;
};
