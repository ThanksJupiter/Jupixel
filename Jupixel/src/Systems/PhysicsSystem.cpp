#include "PhysicsSystem.h"

#include "Player.h"
#include "SkeletonAnimations.h"

#include "glm/glm.hpp"

#include "Components/PhysicsComponent.h"
#include "Components/InputComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/TransformComponent.h"
#include "Components/Components.h"
#include "Components/ActionStateComponent.h"
#include <stdio.h>
#include "Attack.h"
#include "CollisionSystem.h"
#include "Physics/CollisionTestRequest.h"
#include "Animation/Spritesheet.h"
#include <vector>
#include "Components/ColliderComponent.h"

void update_position_system(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;

	switch (state.Action_state)
	{
		case Idle:
			physics_idle_update(player, dt);
			break;
		case Walking:
			physics_walk_update(player, dt);
			break;
		case Attacking:
			physics_attack_update(player, dt);
			break;
		case Jumping:
			physics_jump_update(player, dt);
			break;
		case Falling:
			physics_fall_update(player, dt);
			break;
		default:
			break;
	}
}

void update_position_system(Player* player_one, Player* player_two, float dt)
{
	update_position_system(player_one, dt);
	update_position_system(player_two, dt);
}

void grounded_physics_update(Player* player, float dt)
{
	InputComponent& input = *player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	ActionStateComponent& state = player->ActionState;

	glm::vec2& v = physics.Velocity;

	if (state.Action_state == Attacking)
	{
		v.x = 0.0f;
	}

	transform.Position.x += v.x * dt;
}

void airborne_physics_update(Player* player, float dt)
{
	InputComponent& input = *player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	ActionStateComponent& state = player->ActionState;

	glm::vec2& v = physics.Velocity;

	if (input.Left_stick_x != 0)
	{
		v.x = input.Left_stick_x * 1.0f;
	}

	if (transform.Position.y > -1.0f)
	{
		transform.Position.y += v.y * dt;
		transform.Position.x += v.x * dt;
		v.y -= 10 * dt;

		// HACK cannot be hit in air to prevent is_hit from triggering another jump when landing
		// because of getting hit while jumping
		collider.Is_hit = false;
	}
}

void physics_idle_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	InputComponent& input = *player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;

	glm::vec2& v = physics.Velocity;

	switch (state.Position_state)
	{
		case Grounded:

			physics_set_velocity_to_input(input.Left_stick_x, v.x);
			physics_flip_on_input(input.Left_stick_x, anim);
			grounded_physics_update(player, dt);

			if (input.Left_stick_x != 0)
			{
				set_player_state(player, Walking);
			}

			if (input.Jump)
			{
				v.y += 2;
				transform.Position.y += 0.01;
				set_player_state(player, Airborne);
				set_player_state(player, Jumping);
			}

			if (collider.Is_hit)
			{
				v = collider.Pending_knockback;
				transform.Position.y += 0.01;
				collider.Is_hit = false;
				set_player_state(player, Airborne);
				set_player_state(player, Falling);
			}

			break;
		case Airborne:
			physics_land_on_touch_ground(player);
			airborne_physics_update(player, dt);
			break;
		default:
			break;
	}
}

void physics_walk_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	InputComponent& input = *player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;

	glm::vec2& v = physics.Velocity;

	switch (state.Position_state)
	{
		case Grounded:

			physics_set_velocity_to_input(input.Left_stick_x, v.x);
			physics_apply_velocity_to_position(player, dt);
			physics_flip_on_input(input.Left_stick_x, anim);
			grounded_physics_update(player, dt);

			if (v.x == 0.0f)
			{
				set_player_state(player, Idle);
			}

			if (input.Jump)
			{
				v.y += 2;
				transform.Position.y += 0.01;
				set_player_state(player, Airborne);
				set_player_state(player, Jumping);
			}

			if (collider.Is_hit)
			{
				v = collider.Pending_knockback;
				transform.Position.y += 0.01;
				collider.Is_hit = false;
				set_player_state(player, Airborne);
				set_player_state(player, Falling);
			}


			break;
		case Airborne:
			physics_land_on_touch_ground(player);
			airborne_physics_update(player, dt);
			break;
		default:
			break;
	}
}

void physics_attack_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	CombatComponent& combat = player->Combat;
	AnimationComponent& anim = player->Animation;

	combat.Current_timer += dt;

	if (anim.Current_Sprite_Index == combat.Current_attack->Hitbox_frame)
	{
		CollisionTestRequest request = CollisionTestRequest();
		AnimationComponent anim = player->Animation;

		request.Instigator = player;
		request.Target = player->Opponent;

		ColliderComponent col_comp = player->Combat.Current_attack->Hitbox;
		request.Collider = col_comp;

		request.Collider.Position = player->Transform.Position;
		request.Collider.Offset.x = col_comp.Offset.x * (player->Animation.Is_flipped ? -1 : 1);

		queue_collision(request);
	}

	if (combat.Current_timer >= anim.Current_anim->Duration)
	{
		combat.Current_timer = 0.0f;
		set_player_state(player, Idle);
	}

	if (anim.Current_Sprite_Index == anim.Current_anim->Sprites.size() - 1)
	{
		combat.Current_timer = 0.0f;
		set_player_state(player, Idle);
	}

	switch (state.Position_state)
	{
		case Grounded:
			grounded_physics_update(player, dt);
			break;
		case Airborne:
			physics_land_on_touch_ground(player);
			airborne_physics_update(player, dt);
			break;
		default:
			break;
	}
}

void physics_jump_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;

	switch (state.Position_state)
	{
		case Grounded:
			printf("grounded and jumping, this seems wroong :D");
			break;
		case Airborne:
			physics_land_on_touch_ground(player);
			airborne_physics_update(player, dt);
			break;
		default:
			break;
	}
}

void physics_fall_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	TransformComponent& transform = player->Transform;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;

	glm::vec2& v = physics.Velocity;

	switch (state.Position_state)
	{
		case Grounded:
			printf("grounded and falling, this seems wroong :D");
			break;
		case Airborne:
			
			if (collider.Is_hit)
			{
				v = collider.Pending_knockback;
				transform.Position.y += 0.01;
				collider.Is_hit = false;
				set_player_state(player, Airborne);
				set_player_state(player, Falling);
			}

			physics_land_on_touch_ground(player);
			airborne_physics_update(player, dt);
			break;
		default:
			break;
	}
}

void physics_flip_on_input(float x_input, AnimationComponent& anim)
{
	if (x_input != 0)
	{
		if (x_input > 0 && anim.Is_flipped)
		{
			anim.Is_flipped = false;
		}
		else if (x_input < 0 && !anim.Is_flipped)
		{
			anim.Is_flipped = true;
		}
	}
}

void physics_set_velocity_to_input(float x_input, float& x_velocity)
{
	x_velocity = x_input;
}

void physics_apply_velocity_to_position(Player* player, float dt)
{
	InputComponent& input = *player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	ActionStateComponent& state = player->ActionState;

	glm::vec2& v = physics.Velocity;

	if (input.Left_stick_x != 0)
	{
		v.x = input.Left_stick_x * 2.0f;
	}
	else
	{
		v.x = 0.0f;
	}
}

void physics_apply_drag(Player* player, float dt)
{
	
}

void physics_land_on_touch_ground(Player* player)
{
	TransformComponent& transform = player->Transform;
	PhysicsComponent& physics = player->Physics;
	ActionStateComponent& state = player->ActionState;

	glm::vec2& v = physics.Velocity;

	if (transform.Position.y <= -1.0f)
	{
		v.y = 0;
		transform.Position.y = -1.0f;

		// TODO special case aerial attack l-cancel or summink
		if (state.Action_state != Attacking)
		{
			set_player_state(player, Grounded);
			set_player_state(player, Idle);	
		}
	}
}
