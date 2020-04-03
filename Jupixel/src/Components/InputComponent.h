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

	bool Smash_down = false;
	bool Smash_up = false;
	bool Smash_right = false;
	bool Smash_left = false;

	bool Tilt_down = false;
	bool Tilt_up = false;
	bool Tilt_right = false;
	bool Tilt_left = false;
};
