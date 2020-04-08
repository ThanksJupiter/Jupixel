#pragma once
#include <string>

enum PositionState { Grounded, Airborne, Special };

enum ActionState { Idle, Walking, Running,
	TurnAround, JumpSquat, Attacking,
	Jumping, Falling, Crouching, Airdodge,
	Knockback, Knockdown, Locomotion,
	Ledgegrab, Getup, LedgeBalance, MAX };

//enum LocomotionState { Walking, Running, Jumping, Falling, Knockback, Knockdown, Stationary, MAX };

struct ActionStateComponent
{
	PositionState Position_state = Airborne;
	ActionState Action_state = ActionState::Falling;

	ActionState Previous_action_state = ActionState::MAX;

	int Value = 0;
	float Timer = 0.0f;
	float Time_in_state = 0.0f;
};
