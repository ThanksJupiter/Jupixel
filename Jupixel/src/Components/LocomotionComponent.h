#pragma once

struct LocomotionComponent
{
	bool Is_grounded = false;
	bool Can_ledge_grab = true;

	float Current_ledge_grab_timer = 0.0f;
	const float Ledge_grab_time = 0.6f;

	float Current_dash_timer = 0.0f;
	const float Dash_time = 0.1f;

	float Current_dash_back_timer = 0.0f;
	const float Dash_back_time = 0.5f;

	const float Run_speed = 2.0f;
	const float Walk_speed = 1.0f;

	bool Can_double_jump = false;
	float Current_short_hop_timer = 0.0f;
	const float Short_hop_time = 0.07f;

	float Short_hop_velocity = 3.0f;
	float Full_jump_velocity = 4.5f;

	bool Is_dash_from_walk_allowed = false;

	const float Ledge_balance_threshold = 0.9f;
	bool Ledge_balance_queued = false;

	float Current_get_up_timer = 0.0f;
	const float Auto_get_up = 1.0f;

	bool Has_aerial_control = true;

	bool Can_airdodge = true;
	float Current_airdodge_timer = 0.0f;
	const float Airdodge_time = 0.5f;

	bool Is_turning_allowed = true;
};
