#pragma once

#include <string>

#include "Components/ActionStateComponent.h"

struct Player;

struct State
{
	State() {}

	State(std::string name)
	{
		Name = name;
	}

	std::string Name = "default state";

	ActionState Action_state = ActionState::None;
	PositionState Position_state = PositionState::None;
	LocomotionState Locomotion_state = LocomotionState::None;

	void(*enter)(Player&) = [](Player& p) { printf("Enter default state. Forgot to create state? ;)"); };
	void(*exit)(Player&) = [](Player& p) { printf("Exit default state. Forgot to create state? ;)"); };
	void(*update)(Player&, float) = [](Player& p, float dt) { printf("Update default state. Forgot to create state? ;)"); };
};
