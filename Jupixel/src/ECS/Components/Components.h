#pragma once

#include "Input.h"

struct Position
{
	int entity_id = 0;
	float x = 0;
	float y = 0;
};

struct Velocity
{
	int entity_id = 0;
	float x = 0;
	float y = 0;
};

struct Render
{
	int entity_id = 0;
	int x = 0;
	int y = 0;
};
