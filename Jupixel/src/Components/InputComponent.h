#pragma once

struct InputComponent
{
	int entity_id = 0;
	float Left_stick_x = 0;
	float Left_stick_y = 0;

	bool Jump = false;
	bool Attack = false;
};
