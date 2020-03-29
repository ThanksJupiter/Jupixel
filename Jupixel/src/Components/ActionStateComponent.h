#pragma once

enum PositionState { Grounded, Airborne };
enum ActionState { Idle, Walking, Attacking, Jumping, Falling };

struct ActionStateComponent
{
	PositionState Position_state = Airborne;
	ActionState Action_state = Falling;

	int Value = 0;
	float Timer = 0.0f;
	float Time_in_state = 0.0f;
};
