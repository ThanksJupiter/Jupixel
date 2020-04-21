#include "CombatStates.h"

#include "States/StateThings_incl.h"

State create_grounded_attack_state()
{
	State ret = State("Grounded attack");

	ret.Position_state = PositionState::Grounded;
	ret.Action_state = ActionState::Attacking;

	ret.enter = [](Player& p)
	{
		
	};

	ret.exit = [](Player& p)
	{
		
	};

	ret.update = [](Player& p, float dt)
	{
		combat_perform_attack(p, dt);
	};

	return ret;
}

State create_airborne_attack_state()
{
	State ret = State("Airborne attack");

	ret.Position_state = PositionState::Airborne;
	ret.Action_state = ActionState::Attacking;

	ret.enter = [](Player& p)
	{

	};

	ret.exit = [](Player& p)
	{

	};

	ret.update = [](Player& p, float dt)
	{
		combat_perform_attack(p, dt);
	};

	return ret;
}

void combat_perform_attack(Player& p, float dt)
{
	ActionStateComponent& state = p.ActionState;
	CombatComponent& combat = p.Combat;
	AnimationComponent& anim = p.Animation;
	InputComponent input = p.Input;

	combat.Current_timer += dt;

	if (anim.Current_Sprite_Index >= combat.Current_attack->Hitbox_start_frame &&
		anim.Current_Sprite_Index <= combat.Current_attack->Hitbox_stop_frame &&
		!combat.Is_current_attack_resolved)
	{
		CollisionTestRequest request = CollisionTestRequest();
		AnimationComponent anim = p.Animation;

		request.Instigator = &p;
		request.Target = p.Opponent;

		ColliderComponent col_comp = p.Combat.Current_attack->Hitbox;
		request.Collider = col_comp;

		request.Collider.Position = p.Transform.Position;
		request.Collider.Offset.x = col_comp.Offset.x * (p.Animation.Is_flipped ? -1 : 1);

		queue_collision(request);
	}

	if (anim.Has_full_anim_played)
	{
		combat.Current_timer = 0.0f;
		anim.Has_full_anim_played = false;
		anim.Anim_state = Loop;
		combat.Allow_attacking_movement = false;

		if (state.Position_state == PositionState::Grounded)
		{
			if (p.Locomotion.Ledge_balance_queued && input.Left_stick_x == 0.0f)
			{
				//set_player_state(p, ActionState::LedgeBalance);
				//change_player_animation(p, get_anim(10), Loop);
				p.Locomotion.Ledge_balance_queued = false;
			}
			else
			{
				//set_player_state(p, ActionState::Idle);
				p.Locomotion.Ledge_balance_queued = false;
			}
		}
		else
		{
			//set_player_state(p, ActionState::Falling);
		}
	}
}

