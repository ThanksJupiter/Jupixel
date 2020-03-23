#include "ActionStateSystem.h"

#include "Player.h"

#include "SkeletonAnimations.h"

#include "ECS/Components/InputComponent.h"
#include "Components/ActionStateComponent.h"

void update_action_state_system(Player* player, float dt)
{
	InputComponent* input = player->Input;
	ActionStateComponent& state = player->ActionState;

	if (state.Value != 2)
	{
		if (input->Attack)
		{
			// TODO make animation times something worthwile
			change_player_state(player, 2, 9 * 0.1f);
			change_player_animation(player, get_punch_sheet());
		}
	}
	else
	{
		state.Timer += dt;
		if (state.Timer >= state.Time_in_state)
		{
			change_player_state(player, 0);
			change_player_animation(player, get_idle_sheet());
		}
	}

	switch (state.Value)
	{
		case 0:
			break;

		case 1:
			break;

		case 2:
			break;
	}
}
