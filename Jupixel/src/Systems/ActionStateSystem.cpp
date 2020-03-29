#include "ActionStateSystem.h"

#include "Player.h"

#include "SkeletonAnimations.h"

#include "Components/InputComponent.h"
#include "Components/ActionStateComponent.h"
#include "Physics/CollisionTestRequest.h"
#include "CollisionSystem.h"
#include "Components/CombatComponent.h"
#include "Attack.h"

void update_action_state_system(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;

	if (state.Action_state != Attacking)
	{
		InputComponent* input = player->Input;
		AnimationComponent& anim = player->Animation;
		CombatComponent& combat = player->Combat;

		// TODO if running dash attack etc
		if (state.Position_state == Grounded)
		{
			if (input->Attack)
			{
				player->Physics.Velocity.x = 0.0f;
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[0];
				change_player_animation(player, get_attack_anim(0));
			}
		} 
		else if (state.Position_state == Airborne)
		{
			if (input->Attack)
			{
				set_player_state(player, Attacking);
				// combat_player_attack(Attack_ID);
				combat.Current_attack = &combat.Attacks[1];
				change_player_animation(player, get_attack_anim(1));
			}

			if (input->C_up)
			{
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[2];
				change_player_animation(player, get_attack_anim(2));
			}

			if (input->C_down)
			{
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[3];
				change_player_animation(player, get_attack_anim(3));
			}

			if (input->C_right)
			{
				set_player_state(player, Attacking);
				anim.Is_flipped ? change_player_animation(player, get_attack_anim(5)) :
					change_player_animation(player, get_attack_anim(4));

				anim.Is_flipped ? combat.Current_attack = &combat.Attacks[5] :
					combat.Current_attack = &combat.Attacks[4];
			}

			if (input->C_left)
			{
				set_player_state(player, Attacking);
				anim.Is_flipped ? change_player_animation(player, get_attack_anim(4)) :
					change_player_animation(player, get_attack_anim(5));

				anim.Is_flipped ? combat.Current_attack = &combat.Attacks[4] :
					combat.Current_attack = &combat.Attacks[5];
			}
		}
	}
}

void grounded_update(Player* player, float dt)
{
	 
}

void airborne_update(Player* player, float dt)
{
	
}

void state_idle_update(Player* player, float dt)
{
	
}

void state_walk_update(Player* player, float dt)
{
	
}

void state_attack_update(Player* player, float dt)
{
	
}

void state_jump_update(Player* player, float dt)
{
	
}

void state_fall_update(Player* player, float dt)
{

}

