#pragma once

#include "Systems/ActionStateSystem.h"

#define IDLE_STATE 0
#define WALK_STATE 1
#define ATTACK_STATE 2

struct ActionStateComponent
{
	PositionState Position_state = Airborne;
	ActionState Action_state = Falling;

	int Value = 0;
	float Timer = 0.0f;
	float Time_in_state = 0.0f;
};
