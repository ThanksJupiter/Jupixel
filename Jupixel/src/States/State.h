#pragma once

#include <string>

#include "Components/ActionStateComponent.h"

struct State
{
	std::string name = "default state";

	ActionState Action_state = ActionState::None;
	PositionState Position_state = PositionState::None;
	LocomotionState Locomotion_state = LocomotionState::None;
};
