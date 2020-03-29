#pragma once

struct InputComponent
{
	int entity_id = 0;
	float Left_stick_x = 0;
	float Left_stick_y = 0;

	bool Jump = false;
	bool Attack = false;

	bool C_down = false;
	bool C_up = false;
	bool C_right = false;
	bool C_left = false;
};
