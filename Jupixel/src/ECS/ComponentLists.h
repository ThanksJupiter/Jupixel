#pragma once

#include "Components/Components.h"

struct ComponentLists
{
	PositionComponent position_components[10];
	VelocityComponent velocity_components[10];
	InputComponent input_components[10];
	RenderComponent render_components[10];
	CollisionComponent collision_components[10];

	int total_position_components = 0;
	int total_velocity_components = 0;
	int total_input_components = 0;
	int total_render_components = 0;
	int total_collision_components = 0;
};
