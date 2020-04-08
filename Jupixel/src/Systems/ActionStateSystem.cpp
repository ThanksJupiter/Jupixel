#include "ActionStateSystem.h"

#include "Player.h"
#include "World.h"

#include "SkeletonAnimations.h"

#include "Components/InputComponent.h"
#include "Components/ActionStateComponent.h"
#include "Physics/CollisionTestRequest.h"
#include "CollisionSystem.h"
#include "Components/CombatComponent.h"
#include "Attack.h"
#include "Components/LocomotionComponent.h"

std::string position_state_names[] =
{
	"Grounded",
	"Airborne",
	"Special"
};

std::string action_state_names[] =
{
	"Idle",
	"Walking",
	"Running",
	"TurnAround",
	"JumpSquat",
	"Attacking",
	"Jumping",
	"Falling",
	"Crouching",
	"Airdodge",
	"Knockback",
	"Knockdown",
	"Locomotion",
	"Ledgegrab",
	"Getup",
	"LedgeBalance",
	"Block"
};

void update_action_state_system(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;

	switch (state.Action_state)
	{
		case Idle:
			state_idle_update(player, dt);
			break;
		case Walking:
			state_walk_update(player, dt);
			break;
		case Running:
			state_run_update(player, dt);
			break;
		case Attacking:
			state_attack_update(player, dt);
			break;
		case Jumping:
			break;
		case Falling:
			break;
		case Crouching:
			break;
		case Airdodge:
			break;
		case Knockback:
			break;
		case Locomotion:
			break;
		case MAX:
			break;
		case TurnAround:
			break;
		case JumpSquat:
			state_jump_squat_update(player, dt);
			break;
		default:
			break;
		case Knockdown:
			break;
		case Ledgegrab:
			state_ledgegrab_update(player, dt);
			break;
		case Getup:
			break;
		case LedgeBalance:
			state_ledge_balance_update(player, dt);
			break;
		case Block:
			state_block_update(player, dt);
			break;
	}

	switch (state.Position_state)
	{
		case Grounded:
			state_grounded_update(player, dt);
			break;
		case Airborne:
			state_airborne_update(player, dt);
			break;
		default:
			break;
	}
}

void state_grounded_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	InputComponent input = player->Input;

	if (input.Jump)
	{
		set_player_state(player, JumpSquat);
		change_player_animation(player, get_anim(2), LastFrameStick);
		return;
	}

	if (state.Action_state != Attacking)
	{
		AnimationComponent& anim = player->Animation;
		CombatComponent& combat = player->Combat;

		if (input.Block && state.Action_state != Block)
		{
			player->Physics.Velocity.x = 0.0f;
			set_player_state(player, Block);
			change_player_animation(player, get_anim(11), LastFrameStick);
		}

		if (state.Action_state != Running)
		{
			if (input.Tilt_up)
			{
				player->Physics.Velocity.x = 0.0f;
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[11];
				change_player_animation(player, get_attack_anim(11), LastFrameStick);
				return;
			}

			if (input.Tilt_down)
			{
				player->Physics.Velocity.x = 0.0f;
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[10];
				change_player_animation(player, get_attack_anim(10), LastFrameStick);
				return;
			}

			if (input.Tilt_right)
			{
				player->Physics.Velocity.x = 0.0f;
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[9];
				change_player_animation(player, get_attack_anim(9), LastFrameStick);
				return;
			}

			if (input.Tilt_left)
			{
				// TODO flip if not facing tilt direction / backtilt :00
				player->Physics.Velocity.x = 0.0f;
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[9];
				change_player_animation(player, get_attack_anim(9), LastFrameStick);
				return;
			}

			if (input.Smash_up)
			{
				player->Physics.Velocity.x = 0.0f;
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[7];
				change_player_animation(player, get_attack_anim(7), LastFrameStick);
				return;
			}

			if (input.Smash_down)
			{
				player->Physics.Velocity.x = 0.0f;
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[8];
				change_player_animation(player, get_attack_anim(8), LastFrameStick);
				return;
			}

			if (input.Smash_right)
			{
				if (anim.Is_flipped)
				{
					anim.Is_flipped = false;
				}
				player->Physics.Velocity.x = 0.0f;
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[0];
				change_player_animation(player, get_attack_anim(0), LastFrameStick);
				return;
			}

			if (input.Smash_left)
			{
				if (!anim.Is_flipped)
				{
					anim.Is_flipped = true;
				}
				player->Physics.Velocity.x = 0.0f;
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[0];
				change_player_animation(player, get_attack_anim(0), LastFrameStick);
				return;
			}

			// TODO if running dash attack etc
			if (input.Attack)
			{
				player->Physics.Velocity.x = 0.0f;
				set_player_state(player, Attacking);
				combat.Current_attack = &combat.Attacks[12];
				change_player_animation(player, get_attack_anim(12), LastFrameStick);
				return;
			}
		}

		if (state.Action_state != Crouching)
		{
			if (input.Left_stick_y < -0.8f && state.Action_state != Block)
			{
				player->Physics.Velocity.x = 0.0f;
				set_player_state(player, Crouching);
				change_player_animation(player, get_anim(2), LastFrameStick);
			}
		}
		else
		{
			if (input.Left_stick_y == 0.0f)
			{
				set_player_state(player, Idle);
				change_player_animation(player, get_anim(0), Loop);
			}
		}
	}
}

void state_airborne_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	LocomotionComponent& loco = player->Locomotion;
	TransformComponent& transform = player->Transform;
	PhysicsComponent& physics = player->Physics;

	if (state.Action_state != Attacking)
	{
		AnimationComponent& anim = player->Animation;
		CombatComponent& combat = player->Combat;
		InputComponent input = player->Input;

		if (input.Jump && loco.Can_double_jump)
		{
			loco.Can_double_jump = false;
			physics.Velocity.y = loco.Full_jump_velocity;
			transform.Position.y += 0.01;
			set_player_state(player, Airborne);
			set_player_state(player, Jumping);
			return;
		}

		if (input.Attack)
		{
			set_player_state(player, Attacking);
			combat.Current_attack = &combat.Attacks[1];
			change_player_animation(player, get_attack_anim(1), LastFrameStick);
			return;
		}

		if (input.C_up)
		{
			set_player_state(player, Attacking);
			combat.Current_attack = &combat.Attacks[2];
			change_player_animation(player, get_attack_anim(2), LastFrameStick);
		}

		if (input.C_down)
		{
			set_player_state(player, Attacking);
			combat.Current_attack = &combat.Attacks[3];
			change_player_animation(player, get_attack_anim(3), LastFrameStick);
		}

		if (input.C_right)
		{
			set_player_state(player, Attacking);

			int anim_to_play = anim.Is_flipped ? 5 : 4;

			combat.Current_attack = &combat.Attacks[anim_to_play];
			change_player_animation(player, get_attack_anim(anim_to_play), LastFrameStick);
		}

		if (input.C_left)
		{
			set_player_state(player, Attacking);

			int anim_to_play = anim.Is_flipped ? 4 : 5;

			combat.Current_attack = &combat.Attacks[anim_to_play];
			change_player_animation(player, get_attack_anim(anim_to_play), LastFrameStick);
		}
	}
}

void state_idle_update(Player* player, float dt)
{
	InputComponent input = player->Input;
	LocomotionComponent& loco = player->Locomotion;
	CombatComponent& combat = player->Combat;
	AnimationComponent& anim = player->Animation;

	float ls_x = input.Left_stick_x;

	if (ls_x > 0 && anim.Is_flipped)
	{
		anim.Is_flipped = false;
	}
	else if (ls_x < 0 && !anim.Is_flipped)
	{
		anim.Is_flipped = true;
	}

	if (ls_x != 0)
	{
		loco.Current_dash_timer += dt;

		if (loco.Current_dash_timer >= loco.Dash_time)
		{
			if (abs(ls_x) == 1)
			{
				set_player_state(player, Running);
				change_player_animation(player, get_anim(4), Loop);
				loco.Current_dash_timer = 0.0f;
				loco.Current_dash_back_timer = 0.0f;
			}
			else
			{
				set_player_state(player, Walking);
				loco.Is_dash_from_walk_allowed = false;
			}
		}
		else
		{
			/*if (loco.Current_dash_back_timer <= loco.Dash_back_time)
			{
				set_player_state(player, Running);
				change_player_animation(player, get_anim(4), Loop);
				loco.Current_dash_timer = 0.0f;
				loco.Current_dash_back_timer = 0.0f;
			}*/
		}
	}
	else
	{
		loco.Current_dash_timer = 0.0f;
		loco.Current_dash_back_timer = 0.0f;
	}
}

void state_walk_update(Player* player, float dt)
{
	InputComponent input = player->Input;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	LocomotionComponent& loco = player->Locomotion;

	glm::vec2& v = physics.Velocity;

	float ls_x = input.Left_stick_x;

	//anim.Current_anim->Frame_delay = 0.1f + 1 - (abs(ls_x) * 2.0f);

	if (ls_x > 0 && anim.Is_flipped)
	{
		anim.Is_flipped = false;
		loco.Is_dash_from_walk_allowed = false;
	}
	else if (ls_x < 0 && !anim.Is_flipped)
	{
		anim.Is_flipped = true;
		loco.Is_dash_from_walk_allowed = false;
	}

	if (v.x == 0.0f)
	{
		set_player_state(player, Idle);
	}

	if (abs(ls_x) == 1.0f && loco.Is_dash_from_walk_allowed)
	{
		set_player_state(player, Running);
		change_player_animation(player, get_anim(4), Loop);
		loco.Current_dash_timer = 0.0f;
		loco.Current_dash_back_timer = 0.0f;
	}

	if (ls_x == 0.0f)
	{
		loco.Is_dash_from_walk_allowed = true;
	}
}

void state_run_update(Player* player, float dt)
{
	InputComponent input = player->Input;
	LocomotionComponent& loco = player->Locomotion;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;

	glm::vec2& v = physics.Velocity;
	float ls_x = input.Left_stick_x;

	loco.Current_dash_back_timer += dt;

	if (input.Attack)
	{
		CombatComponent& combat = player->Combat;
		combat.Allow_attacking_movement = true;
		set_player_state(player, Attacking);
		change_player_animation(player, get_attack_anim(6), LastFrameStick);
		combat.Current_attack = &combat.Attacks[6];
	}

	bool flipped = false;
	if (loco.Current_dash_back_timer <= loco.Dash_back_time)
	{
		if (ls_x > 0 && anim.Is_flipped)
		{
			anim.Is_flipped = false;
			flipped = true;
		}
		else if (ls_x < 0 && !anim.Is_flipped)
		{
			anim.Is_flipped = true;
			flipped = true;
		}

		if (v.x == 0.0f && ls_x == 0.0f)
		{
			set_player_state(player, Walking);
			loco.Is_dash_from_walk_allowed = false;
			loco.Current_dash_back_timer = 0.0f;
			return;
		}
	}
	else
	{
		if (ls_x > 0 && anim.Is_flipped)
		{
			set_player_state(player, TurnAround);
			change_player_animation(player, get_anim(5), LastFrameStick);
			flipped = true;
		}
		else if (ls_x < 0 && !anim.Is_flipped)
		{
			set_player_state(player, TurnAround);
			change_player_animation(player, get_anim(5), LastFrameStick);
			flipped = true;
		}

		if (abs(v.x) < 0.5f && ls_x == 0.0f)
		{
			set_player_state(player, Walking);
			loco.Is_dash_from_walk_allowed = false;
			loco.Current_dash_back_timer = 0.0f;
		}
	}

	if (flipped)
	{
		loco.Current_dash_back_timer = 0.0f;
		loco.Is_dash_from_walk_allowed = false;
		return;
	}
}

void state_jump_squat_update(Player* player, float dt)
{
	LocomotionComponent& loco = player->Locomotion;
	InputComponent input = player->Input;
	TransformComponent& transform = player->Transform;
	PhysicsComponent& physics = player->Physics;

	glm::vec2& v = physics.Velocity;
	v.x = 0.0f;

	loco.Current_short_hop_timer += dt;

	if (!input.Jump_held)
	{
		v.y += loco.Short_hop_velocity;
		transform.Position.y += 0.01;
		loco.Current_short_hop_timer = 0.0f;
		set_player_state(player, Airborne);
		set_player_state(player, Jumping);
		return;
	}

	if (loco.Current_short_hop_timer >= loco.Short_hop_time)
	{
		v.y += loco.Full_jump_velocity;
		transform.Position.y += 0.01;
		loco.Current_short_hop_timer = 0.0f;
		set_player_state(player, Airborne);
		set_player_state(player, Jumping);
	}
}

void state_attack_update(Player* player, float dt)
{
	CombatComponent& combat = player->Combat;
	AnimationComponent& anim = player->Animation;
	InputComponent input = player->Input;

	if (combat.Current_attack == &combat.Attacks[12])
	{
		if (anim.Current_Sprite_Index > 1 && anim.Current_Sprite_Index < 3)
		{
			if (input.Attack)
			{
				CombatComponent& combat = player->Combat;
				combat.Allow_attacking_movement = true;
				set_player_state(player, Attacking);
				change_player_animation(player, get_attack_anim(12), LastFrameStick);
				anim.Current_Sprite_Index = 3;
				combat.Current_attack = &combat.Attacks[13];
			}
		}
		else if (anim.Current_Sprite_Index >= 2)
		{
			set_player_state(player, Idle);
			change_player_animation(player, get_anim(0));
		}
	}
}

void state_jump_update(Player* player, float dt)
{

}

void state_fall_update(Player* player, float dt)
{

}

void state_ledgegrab_update(Player* player, float dt)
{
	InputComponent input = player->Input;

	if (player->Animation.Current_Sprite_Index <= 1)
	{
		return;
	}

	if (!player->Animation.Is_flipped && input.Left_stick_x < -0.9f)
	{
		player->Locomotion.Can_double_jump = true;
		player->Locomotion.Can_ledge_grab = false;
		player->Locomotion.Current_ledge_grab_timer = 0.0f;
		set_player_state(player, Falling);
		set_player_state(player, Airborne);
	}
	else if (player->Animation.Is_flipped && input.Left_stick_x > 0.9f)
	{
		player->Locomotion.Can_double_jump = true;
		player->Locomotion.Can_ledge_grab = false;
		player->Locomotion.Current_ledge_grab_timer = 0.0f;
		set_player_state(player, Falling);
		set_player_state(player, Airborne);
	}
}

void state_ledge_balance_update(Player* player, float dt)
{
	InputComponent input = player->Input;
	LocomotionComponent& loco = player->Locomotion;
	CombatComponent& combat = player->Combat;
	AnimationComponent& anim = player->Animation;

	float ls_x = input.Left_stick_x;

	if (ls_x != 0)
	{
		loco.Current_dash_timer += dt;

		if (loco.Current_dash_timer >= loco.Dash_time)
		{
			if (anim.Is_flipped) // if facing left
			{
				if (ls_x < -loco.Ledge_balance_threshold) // if input left (walk)
				{
					set_player_state(player, Walking);
					loco.Is_dash_from_walk_allowed = false;

				}
				else if (ls_x == -1.0f) // if input left (run)
				{
					set_player_state(player, Running);
					change_player_animation(player, get_anim(4), Loop);
					loco.Current_dash_timer = 0.0f;
					loco.Current_dash_back_timer = 0.0f;
				}
				else if (ls_x > loco.Ledge_balance_threshold) // if input right (walk)
				{
					anim.Is_flipped = false;
					set_player_state(player, Walking);
					loco.Is_dash_from_walk_allowed = false;
				}
				else if (ls_x == 1.0f) // if input right (run)
				{
					anim.Is_flipped = false;
					set_player_state(player, Running);
					change_player_animation(player, get_anim(4), Loop);
					loco.Current_dash_timer = 0.0f;
					loco.Current_dash_back_timer = 0.0f;
				}
			}
			else // if facing right
			{
				if (ls_x > loco.Ledge_balance_threshold && ls_x < 1.0f) // if input right (walk)
				{
					set_player_state(player, Walking);
					loco.Is_dash_from_walk_allowed = false;
				}
				else if (ls_x == 1.0f) // if input right (run)
				{
					set_player_state(player, Running);
					change_player_animation(player, get_anim(4), Loop);
					loco.Current_dash_timer = 0.0f;
					loco.Current_dash_back_timer = 0.0f;
				}
				else if (ls_x < -loco.Ledge_balance_threshold && ls_x != -1.0f) // if input left (walk)
				{
					anim.Is_flipped = true;
					set_player_state(player, Walking);
					loco.Is_dash_from_walk_allowed = false;
				}
				else if (ls_x == -1.0f) // if input left (run)
				{
					anim.Is_flipped = true;
					set_player_state(player, Running);
					change_player_animation(player, get_anim(4), Loop);
					loco.Current_dash_timer = 0.0f;
					loco.Current_dash_back_timer = 0.0f;
				}
			}
		}
	}
	else
	{
		loco.Current_dash_timer = 0.0f;
		loco.Current_dash_back_timer = 0.0f;
	}
}

void state_block_update(Player* player, float dt)
{
	InputComponent input = player->Input;

	if (!input.Block)
	{
		set_player_state(player, Idle);
	}

	if (input.Left_stick_y < -0.3f && player->Combat.High_block)
	{
		change_player_animation(player, get_anim(12), LastFrameStick);
		player->Combat.High_block = false;
	}
	else if	(input.Left_stick_y == 0.0f && !player->Combat.High_block)
	{
		change_player_animation(player, get_anim(11), LastFrameStick);
		player->Animation.Current_Sprite_Index = 3;
		player->Combat.High_block = true;
	}
}

const char* get_position_state_name(int id)
{
	return position_state_names[id].c_str();
}

const char* get_action_state_name(int id)
{
	return action_state_names[id].c_str();
}
