#include "Systems/StateSystem.h"

#include "Player.h"

void state_initialize(Player& player)
{
	

	player.StateC.Current_position_state = get_states().Airborne;
	player.StateC.Current_locomotion_state = get_states().Idle;
	player.StateC.Current_action_state = get_states().Special;
}

void state_update(Player& player, float dt)
{
	player.StateC.Time_in_position_state += dt;
	player.StateC.Current_position_state.update(player, dt);

	player.StateC.Time_in_action_state += dt;
	player.StateC.Current_action_state.update(player, dt);

	player.StateC.Time_in_locomotion_state += dt;
	player.StateC.Current_locomotion_state.update(player, dt);
}
