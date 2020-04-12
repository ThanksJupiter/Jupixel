#pragma once
#include <string>

enum class PositionState { Grounded, Airborne, Special, None };

enum class ActionState { Idle, Walking, Running,
	TurnAround, JumpSquat, Attacking,
	Jumping, Falling, Crouching, Airdodge,
	Knockback, Knockdown, Locomotion,
	Ledgegrab, Getup, LedgeBalance,
	Block, None };

enum class LocomotionState { Walking, Running, Jumping, Falling, Knockback, Knockdown, Stationary, None };

struct ActionStateComponent
{
	PositionState Position_state = PositionState::Airborne;
	ActionState Action_state = ActionState::Falling;

	ActionState Previous_action_state = ActionState::None;

	int Value = 0;
	float Timer = 0.0f;
	float Time_in_state = 0.0f;
};
