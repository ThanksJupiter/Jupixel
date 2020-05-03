#include "jppch.h"

#include "AISystem.h"

#include "Player.h"
#include "World.h"

#include "SkeletonAnimations.h"

#include "Components/InputComponent.h"
#include "Components/ActionStateComponent.h"
#include "Physics/CollisionTestRequest.h"-
#include "CollisionSystem.h"
#include "Components/CombatComponent.h"
#include "Attack.h"
#include "Components/LocomotionComponent.h"

#include "States/State.h"

const float run_dst = 0.8f;
const float medium_dst = 0.6f;
const float short_dst = 0.4f;

void update_ai_system(Player& player, float dt)
{
	ActionStateComponent& state = player.ActionState;
	InputComponent& input = player.Input;

	input.Attack = false;
	input.Smash_left = false;
	input.Smash_right = false;

	switch (state.Action_state)
	{
		case ActionState::Idle:
			approach_player(player, dt);
			break;
		case ActionState::Walking:
			approach_player(player, dt);
			break;
		case ActionState::Running:
			approach_player(player, dt);
			break;
		case ActionState::TurnAround:
			approach_player(player, dt);
			break;
		case ActionState::JumpSquat:
			break;
		case ActionState::Attacking:
			break;
		case ActionState::Jumping:
			approach_player(player, dt);
			break;
		case ActionState::Falling:
			//approach_player(player, dt);
			break;
		case ActionState::Crouching:
			approach_player(player, dt);
			break;
		case ActionState::Airdodge:
			break;
		case ActionState::Knockback:
			//approach_player(player, dt);
			break;
		case ActionState::Knockdown:
			break;
		case ActionState::Locomotion:
			break;
		case ActionState::Ledgegrab:
			break;
		case ActionState::Getup:
			break;
		case ActionState::LedgeBalance:
			approach_player(player, dt);
			break;
		case ActionState::Block:
			approach_player(player, dt);
			break;
		case ActionState::Locomoting:
			break;
		case ActionState::None:
			break;
		default:
			break;
	}
}

void approach_player(Player& player, float dt)
{
	InputComponent& input = player.Input;
	ActionStateComponent& state = player.ActionState;

	Player& opponent = *player.Opponent;
	ActionStateComponent& opponent_state = opponent.ActionState;

	float dst_to_target = glm::distance(player.Transform.Position, opponent.Transform.Position);

	if (opponent_state.Action_state == ActionState::Attacking && dst_to_target < 1.0f)
	{
		input.Block = true;
		input.Attack = false;
		input.Smash_left = false;
		input.Smash_right = false;
		return;
	}
	else
	{
		input.Block = false;
		input.Attack = false;
	}

	/*if (opponent_state.Action_state == ActionState::Block)
	{
		input.Left_stick_x = 0.0f;
		input.Left_stick_y = -1.0f;
		return;
	}
	else
	{
		input.Left_stick_y = 0.0f;
	}*/

	input.Smash_left = false;
	input.Smash_right = false;

	glm::vec2 dir_to_target = opponent.Transform.Position - player.Transform.Position;
	float x_dir = dir_to_target.x;

	if (dst_to_target > run_dst)
	{
		input.Left_stick_x = (x_dir > 0.0f ? 1.0f : -1.0f);

		if (state.Action_state == ActionState::Walking)
		{
			input.Left_stick_x = 0.0f;
		}
	}
	else if (dst_to_target < run_dst && dst_to_target > medium_dst)
	{
		input.Left_stick_x = (x_dir > 0.0f ? 1.0f : -0.4f);
	} 
	else if (dst_to_target < medium_dst && dst_to_target > short_dst)
	{
		AnimationComponent& anim = player.Animation;
		CombatComponent& combat = player.Combat;

		if (x_dir > 0.0f && anim.Is_flipped)
		{
			anim.Is_flipped = false;
		}
		else if (x_dir < 0.0f && !anim.Is_flipped)
		{
			anim.Is_flipped = true;
		}

		attack_medium_range(player, dt);
	}
	else if (dst_to_target < short_dst)
	{
		AnimationComponent& anim = player.Animation;
		CombatComponent& combat = player.Combat;

		if (x_dir > 0.0f && anim.Is_flipped)
		{
			anim.Is_flipped = false;
		}
		else if (x_dir < 0.0f && !anim.Is_flipped)
		{
			anim.Is_flipped = true;
		}

		attack_close_range(player, dt);
	}
}

void attack_close_range(Player& player, float dt)
{
	AnimationComponent& anim = player.Animation;
	CombatComponent& combat = player.Combat;
	InputComponent& input = player.Input;
	ActionStateComponent& state = player.ActionState;

	Player& opponent = *player.Opponent;
	ActionStateComponent& opponent_state = opponent.ActionState;

	input.Attack = true;
	input.Left_stick_x = 0.0f;
}

void attack_medium_range(Player& player, float dt)
{
	AnimationComponent& anim = player.Animation;
	CombatComponent& combat = player.Combat;
	InputComponent& input = player.Input;
	ActionStateComponent& state = player.ActionState;

	Player& opponent = *player.Opponent;
	ActionStateComponent& opponent_state = opponent.ActionState;

	glm::vec2 dir_to_target = opponent.Transform.Position - player.Transform.Position;
	float x_dir = dir_to_target.x;

	if (x_dir > 0.0f)
	{
		input.Smash_right = true;
	}
	else if (x_dir < 0.0f)
	{
		input.Smash_left = true;
	}

	if (state.Action_state == ActionState::Running)
	{
		input.Attack = true;
	}

	input.Left_stick_x = 0.0f;
}
