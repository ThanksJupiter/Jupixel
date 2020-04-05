#pragma once

struct LocomotionComponent
{
	bool Is_grounded = false;
	bool Can_ledge_grab = true;

	float Current_ledge_grab_timer = 0.0f;
	float Ledge_grab_time = 0.6f;

	float Current_dash_timer = 0.0f;
	float Dash_time = 0.1f;

	float Current_dash_back_timer = 0.0f;
	float Dash_back_time = 0.5f;

	float Run_speed = 2.0f;
	float Walk_speed = 1.0f;

	bool Can_double_jump = false;
	float Current_short_hop_timer = 0.0f;
	float Short_hop_time = 0.15f;

	float Short_hop_velocity = 2.0f;
	float Full_jump_velocity = 4.0f;

	bool Is_dash_from_walk_allowed = false;
};
