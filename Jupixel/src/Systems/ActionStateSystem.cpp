#include "ActionStateSystem.h"

#include "Player.h"

#include "SkeletonAnimations.h"

#include "Components/InputComponent.h"
#include "Components/ActionStateComponent.h"
#include "Physics/CollisionTestRequest.h"
#include "CollisionSystem.h"

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
		else if (state.Timer >= 0.6f) // TODO don't magic number the attack hitbox checking
		{
			CollisionTestRequest* request = new CollisionTestRequest();

			ColliderComponent col_comp = ColliderComponent();
			// TODO know which attack should be used for collider position & scale here
			col_comp.Position = player->Transform.Position;
			col_comp.Scale = player->Transform.Scale;
			request->Collider = col_comp;

			request->Instigator = player;
			request->Target = player->Opponent;

			queue_collision(request);
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
