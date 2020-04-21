#pragma once

#include "States/State.h"

#include "Systems/StateSystem.h"
#include "Player.h"
#include "../States/PlayerStates.h"

struct StateComponent
{
	State Current_position_state;
	State Previous_position_state;

	State Current_action_state;
	State Previous_action_state;

	State Current_locomotion_state;
	State Previous_locomotion_state;

	float Time_in_position_state = 0.0f;
	float Time_in_action_state = 0.0f;
	float Time_in_locomotion_state = 0.0f;

	void set_knockback_state(Player& player);
};

/*
void StateComponent::set_knockback_state(Player& player)
{
	player.set_action_state(get_states().Idle);
}*/
