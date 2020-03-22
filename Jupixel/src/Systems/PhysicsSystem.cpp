#include "PhysicsSystem.h"

#include "Player.h"
#include "SkeletonAnimations.h"

#include "glm/glm.hpp"

#include "Components/PhysicsComponent.h"
#include "ECS/Components/InputComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/TransformComponent.h"
#include "ECS/Components/Components.h"

void update_position_system(Player* _player, float _dt)
{
	InputComponent& input = *_player->input;
	TransformComponent& transform = _player->transform;
	AnimationComponent& anim = _player->animation;
	PhysicsComponent& physics = _player->physics;
	ColliderComponent& collider = _player->collider;

	glm::vec2& v = physics.Velocity;

	if (input.left_stick_x != 0)
	{
		if (input.left_stick_x > 0 && anim.isFlipped)
		{
			anim.isFlipped = false;
			transform.Scale.x = -transform.Scale.x;
		}
		else if (input.left_stick_x < 0 && !anim.isFlipped)
		{
			anim.isFlipped = true;
			transform.Scale.x = -transform.Scale.x;
		}

		v.x = input.left_stick_x * 2.0f;
		anim.current_anim = get_walk_sheet();
	}
	else
	{
		anim.current_anim = get_idle_sheet();
		v.x = 0;
	}

	transform.Position.x += v.x * _dt;

	if (transform.Position.y > -1.0f)
	{
		v.y -= 10 * _dt;
		transform.Position.y += v.y * _dt;

		// HACK cannot be hit in air to prevent is_hit from triggering another jump when landing
		// because of getting hit while jumping
		collider.is_hit = false;
	}
	else
	{
		v.y = 0;
		transform.Position.y = -1.0f;

		if (input.jump || collider.is_hit)
		{
			v.y += 5;
			transform.Position.y += 0.01;
			collider.is_hit = false;
		}
	}
}
