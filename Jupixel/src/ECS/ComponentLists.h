#pragma once

#include "Components/Components.h"

struct ComponentLists
{
	Position position_components[10];
	Velocity velocity_components[10];
	Input input_components[10];
	Render render_components[10];

	int total_position_components = 0;
	int total_velocity_components = 0;
	int total_input_components = 0;
	int total_render_components = 0;
};
