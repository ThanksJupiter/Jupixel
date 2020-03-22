#pragma once

struct InputComponent
{
	int entity_id = 0;
	float left_stick_x = 0;
	float left_stick_y = 0;

	bool jump = false;
	bool attack = false;
};
