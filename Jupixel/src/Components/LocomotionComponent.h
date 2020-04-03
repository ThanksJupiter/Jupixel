#pragma once

struct LocomotionComponent
{
	float Current_dash_timer = 0.0f;
	float Dash_time = 0.1f;

	float Current_dash_back_timer = 0.0f;
	float Dash_back_time = 0.5f;

	float Run_speed = 2.0f;
	float Walk_speed = 1.0f;

	float Current_short_hop_timer = 0.0f;
	float Short_hop_time = 0.15f;

	float Short_hop_velocity = 2.0f;
	float Full_jump_velocity = 4.0f;

	bool Is_dash_from_walk_allowed = false;
};
