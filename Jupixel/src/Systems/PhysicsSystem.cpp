#include "PhysicsSystem.h"

#include "Player.h"
#include "SkeletonAnimations.h"

#include "glm/glm.hpp"

#include "Components/PhysicsComponent.h"
#include "ECS/Components/InputComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/TransformComponent.h"
#include "ECS/Components/Components.h"
#include "Components/ActionStateComponent.h"

void update_position_system(Player* player, float dt)
{
	InputComponent& input = *player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	ActionStateComponent& action_state = player->ActionState;

	glm::vec2& v = physics.Velocity;

	if (input.Left_stick_x != 0)
	{
		if (input.Left_stick_x > 0 && anim.Is_flipped)
		{
			anim.Is_flipped = false;
		}
		else if (input.Left_stick_x < 0 && !anim.Is_flipped)
		{
			anim.Is_flipped = true;
		}

		v.x = input.Left_stick_x * 2.0f;

		if (action_state.Value == 0)
		{
			change_player_state(player, 1);
			change_player_animation(player, get_walk_sheet());
		}
	}
	else
	{
		if (action_state.Value == 1)
		{
			change_player_state(player, 0);
			change_player_animation(player, get_idle_sheet());
		}
		v.x = 0;
	}

	transform.Position.x += v.x * dt;

	if (transform.Position.y > -1.0f)
	{
		v.y -= 10 * dt;
		transform.Position.y += v.y * dt;

		// HACK cannot be hit in air to prevent is_hit from triggering another jump when landing
		// because of getting hit while jumping
		collider.is_hit = false;
	}
	else
	{
		v.y = 0;
		transform.Position.y = -1.0f;

		if (input.Jump || collider.is_hit)
		{
			v.y += 5;
			transform.Position.y += 0.01;
			collider.is_hit = false;
		}
	}
}
