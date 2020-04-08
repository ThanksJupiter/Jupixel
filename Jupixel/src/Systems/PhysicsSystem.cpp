#include "PhysicsSystem.h"

#include "World.h"
#include "Player.h"
#include "SkeletonAnimations.h"
#include "Animation/Spritesheet.h"
#include "Attack.h"
#include "Physics/Raycaster.h"

#include "Components/PhysicsComponent.h"
#include "Components/InputComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/TransformComponent.h"
#include "Components/Components.h"
#include "Components/ActionStateComponent.h"
#include "CollisionSystem.h"
#include "Physics/CollisionTestRequest.h"
#include "Components/ColliderComponent.h"

#include <stdio.h>
#include <vector>
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "Renderer/Renderer.h"
#include "VFXSystem.h"

const float knockback_scale_factor = 0.1f;

void update_physics_system(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;

	if (get_time_scale() < 1.0f)
	{
		set_time_scale(get_time_scale() + 120.0f * dt);
	}
	else
	{
		reset_time_scale();
	}

	switch (state.Action_state)
	{
		case Idle:
			physics_idle_update(player, dt);
			break;
		case Walking:
			physics_walk_update(player, dt);
			break;
		case Running:
			physics_run_update(player, dt);
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
		case Crouching:
			physics_crouch_update(player, dt);
			break;
		case Airdodge:
			break;
		case Knockback:
			physics_knockback_update(player, dt);
			break;
		case MAX:
			break;
		default:
			printf("invalid action state");
			break;
		case Locomotion:
			break;
		case TurnAround:
			physics_turn_around_update(player, dt);
			break;
		case JumpSquat:
			break;
		case Knockdown:
			physics_knockdown_update(player, dt);
			break;
		case Ledgegrab:
			
			break;
		case Getup:
			physics_getup_update(player, dt);
			break;
		case LedgeBalance:
			physics_ledge_balance_update(player, dt);
			break;
	}

	if (state.Position_state == PositionState::Special)
	{
		special_physics_update(player, dt);
	}
}

void update_physics_system(Player* player_one, Player* player_two, float dt)
{
	update_physics_system(player_one, dt);
	update_physics_system(player_two, dt);
}

void grounded_physics_update(Player* player, float dt)
{
	InputComponent input = player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	ActionStateComponent& state = player->ActionState;
	CombatComponent& combat = player->Combat;
	LocomotionComponent& loco = player->Locomotion;

	glm::vec2& v = physics.Velocity;

	// TODO remove no x-velocity hack when attacking on ground
	if (state.Action_state == Attacking && !combat.Allow_attacking_movement)
	{
		v.x = 0.0f;
	}

	if (collider.Is_hit)
	{
		combat.Current_health_percentage += collider.Pending_damage;
		collider.Pending_damage = 0.0f;

		v = state.Action_state == ActionState::Crouching ? collider.Pending_knockback * 0.3f : collider.Pending_knockback;

		if (v.y < 0.0f)
		{
			v.y = -v.y;
		}

		loco.Current_get_up_timer = 0.0f;
		v += v * combat.Current_health_percentage  * knockback_scale_factor;
		glm::vec2 printV = v;
		printf("Knockback: %s\n", glm::to_string(printV).c_str());

		transform.Position.y += 0.01;
		collider.Is_hit = false;
		anim.Is_flipped = collider.Flip;
		set_player_state(player, Airborne);
		set_player_state(player, Knockback);
		change_player_animation(player, get_anim(3), LastFrameStick);
	}

	RaycastHit hit = RaycastHit();
	if (raycast(transform.Position, glm::vec2(0.0f, -1.0f), 0.1f, hit))
	{
		transform.Position.x += v.x * dt;
	}
	else // TODO know if *is about* to walk off edge
	{
		if (state.Action_state == Walking && abs(input.Left_stick_x) < loco.Ledge_balance_threshold)
		{
			transform.Position.x -= v.x * dt;
			v.x = 0.0f;

			set_player_state(player, LedgeBalance);
			change_player_animation(player, get_anim(10), Loop);
			return;
		}

		if (state.Action_state != Attacking)
		{
			set_player_state(player, Airborne);
			set_player_state(player, Falling);
			return;
		}
		else
		{
			transform.Position.x -= v.x * dt;
			v.x = 0.0f;
			loco.Ledge_balance_queued = true;
		}
	}
}

void airborne_physics_update(Player* player, float dt)
{
	InputComponent& input = player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	ActionStateComponent& state = player->ActionState;

	if (state.Action_state != Ledgegrab)
	{
		player->Locomotion.Current_ledge_grab_timer += dt;

		if (player->Locomotion.Current_ledge_grab_timer >= player->Locomotion.Ledge_grab_time)
		{
			player->Locomotion.Current_ledge_grab_timer = 0.0f;
			player->Locomotion.Can_ledge_grab = true;
		}
	}

	glm::vec2& v = physics.Velocity;

	if (input.Left_stick_x != 0)
	{
		v.x = input.Left_stick_x * 1.0f;
	}

	if (state.Action_state != Attacking)
	{
		physics_check_grab_ledge(player, dt);
	}

	if (collider.Is_hit)
	{
		CombatComponent& combat = player->Combat;
		combat.Current_health_percentage += collider.Pending_damage;
		collider.Pending_damage = 0.0f;

		v.x = collider.Pending_knockback.x;

		if (v.y < 0)
		{
			if (collider.Pending_knockback.y < 0)
			{
				v.y += collider.Pending_knockback.y;
			}
			else
			{
				v.y = collider.Pending_knockback.y;
			}
		}

		v += v * combat.Current_health_percentage * knockback_scale_factor;
		glm::vec2 printV = v;
		printf("Knockback: %s\n", glm::to_string(printV).c_str());

		collider.Is_hit = false;
		anim.Is_flipped = collider.Flip;
		set_player_state(player, Airborne);
		set_player_state(player, Knockback);
		change_player_animation(player, get_anim(3), LastFrameStick);
	}

	glm::vec2 origin = glm::vec2(transform.Position.x, transform.Position.y + 16 * 0.02f);
	glm::vec2 direction = glm::vec2(v.x, 0.0f);
	float distance = 0.05f;

	//render_quad(origin + (direction * distance), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(0.1f, 0.1f));
	RaycastHit hit = RaycastHit();
	if (raycast(origin, direction, distance, hit))
	{
		transform.Position.x = hit.point.x + (v.x > 0 ? (-2 * 0.02f) : 2 * 0.02f);
		v.x = 0.0f;
	}
	else
	{
		transform.Position.x += v.x * dt;
	}

	glm::vec2 up_dir = glm::vec2(0.0f, 1.0f);

	//render_quad((origin - direction * distance) + (up_dir * distance), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.1f, 0.1f));
	if (raycast(origin - direction * distance, up_dir, distance, hit))
	{
		transform.Position.y = hit.point.y - 42 * 0.02f;
		v.y = 0.0f;
	}
	else
	{
		transform.Position.y += v.y * dt;
	}

	v.y -= 10 * dt;
}

void special_physics_update(Player* player, float dt)
{
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	ActionStateComponent& state = player->ActionState;
	CombatComponent& combat = player->Combat;

	glm::vec2& v = physics.Velocity;

	if (collider.Is_hit)
	{
		combat.Current_health_percentage += collider.Pending_damage;
		collider.Pending_damage = 0.0f;

		v = collider.Pending_knockback;

		v += v * combat.Current_health_percentage  * knockback_scale_factor;
		glm::vec2 printV = v;
		printf("Knockback: %s\n", glm::to_string(printV).c_str());

		transform.Position.y += 0.01;
		collider.Is_hit = false;
		anim.Is_flipped = collider.Flip;
		set_player_state(player, Airborne);
		set_player_state(player, Knockback);
		change_player_animation(player, get_anim(3), LastFrameStick);
	}
}

void physics_idle_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	InputComponent& input = player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;

	glm::vec2& v = physics.Velocity;

	switch (state.Position_state)
	{
		case Grounded:

			physics_add_input_to_velocity(input.Left_stick_x, v.x, dt);
			physics_flip_on_input(player, dt);
			physics_apply_drag(player, dt);
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

void physics_walk_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	InputComponent& input = player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	LocomotionComponent loco = player->Locomotion;

	glm::vec2& v = physics.Velocity;

	switch (state.Position_state)
	{
		case Grounded:

			//physics_add_velocity_to_input(input.Left_stick_x * loco.Run_speed, v.x, dt);
			physics_apply_input_to_velocity(player, dt);
			physics_flip_on_input(player, dt);
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

void physics_run_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	InputComponent& input = player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	LocomotionComponent loco = player->Locomotion;

	glm::vec2& v = physics.Velocity;

	switch (state.Position_state)
	{
		case Grounded:

			physics_set_velocity_to_input(input.Left_stick_x * loco.Run_speed, v.x);
			physics_apply_input_to_velocity(player, dt);
			//physics_flip_on_input(player, dt);
			grounded_physics_update(player, dt);

			// TODO ground physics update before state updates?

			break;
		case Airborne:
			physics_land_on_touch_ground(player);
			airborne_physics_update(player, dt);
			break;
		default:
			break;
	}
}

void physics_turn_around_update(Player* player, float dt)
{
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;

	glm::vec2& v = physics.Velocity;

	if (anim.Has_full_anim_played)
	{
		anim.Is_flipped = !anim.Is_flipped;
		set_player_state(player, Idle);
	}

	//physics_add_velocity_to_input(player->Input.Left_stick_x, v.x, dt);
	physics_apply_input_to_velocity(player, dt);
	grounded_physics_update(player, dt);
}

void physics_attack_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	CombatComponent& combat = player->Combat;
	AnimationComponent& anim = player->Animation;
	InputComponent input = player->Input;

	combat.Current_timer += dt;

	if (anim.Current_Sprite_Index == combat.Current_attack->Hitbox_frame && !combat.Is_current_attack_resolved)
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

	if (anim.Has_full_anim_played)
	{
		combat.Current_timer = 0.0f;
		anim.Has_full_anim_played = false;
		anim.Anim_state = Loop;
		combat.Allow_attacking_movement = false;

		if (state.Position_state == Grounded)
		{
			if (player->Locomotion.Ledge_balance_queued && input.Left_stick_x == 0.0f)
			{
				set_player_state(player, LedgeBalance);
				change_player_animation(player, get_anim(10), Loop);
				player->Locomotion.Ledge_balance_queued = false;
			}
			else
			{
				set_player_state(player, Idle);
				player->Locomotion.Ledge_balance_queued = false;
			}
		}
		else
		{
			set_player_state(player, Falling);
		}
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
	PhysicsComponent& physics = player->Physics;

	glm::vec2& v = physics.Velocity;

	switch (state.Position_state)
	{
		case Grounded:
			printf("grounded and jumping, this seems wroong :D");
			break;
		case Airborne:

			if (v.y < 0)
			{
				physics_land_on_touch_ground(player);
			}

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
	AnimationComponent& anim = player->Animation;

	glm::vec2& v = physics.Velocity;

	switch (state.Position_state)
	{
		case Grounded:
			printf("grounded and falling, this seems wroong :D");
			break;
		case Airborne:
			physics_land_on_touch_ground(player);
			airborne_physics_update(player, dt);
			break;
		default:
			break;
	}
}

void physics_crouch_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	TransformComponent& transform = player->Transform;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	AnimationComponent& anim = player->Animation;

	glm::vec2& v = physics.Velocity;

	switch (state.Position_state)
	{
		case Grounded:

			grounded_physics_update(player, dt);
			break;
		case Airborne:

			printf("crouching in mid air? hmm ;)");
			break;
		default:
			break;
	}
}

void physics_knockback_update(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;
	TransformComponent& transform = player->Transform;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	AnimationComponent& anim = player->Animation;

	glm::vec2& v = physics.Velocity;

	anim.Current_dust_timer += dt;

	if (anim.Current_dust_timer >= anim.Dust_time)
	{
		anim.Current_dust_timer = 0.0;
		vfx_spawn_effect(get_vfx_anim(2), transform.Position + glm::vec2(0.0f, 0.3f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
	}

	switch (state.Position_state)
	{
		case Grounded:
			grounded_physics_update(player, dt);
			break;
		case Airborne:

			airborne_physics_update(player, dt);
			physics_land_on_touch_ground(player);
			break;
		default:
			break;
	}
}

void physics_knockdown_update(Player* player, float dt)
{
	LocomotionComponent& loco = player->Locomotion;

	loco.Current_get_up_timer += dt;

	if (loco.Current_get_up_timer >= loco.Auto_get_up)
	{
		loco.Current_get_up_timer = 0.0f;
		set_player_state(player, Getup);
		change_player_animation(player, get_anim(9), LastFrameStick);
	}

	physics_apply_drag(player, dt);
	grounded_physics_update(player, dt);
}

void physics_getup_update(Player* player, float dt)
{
	AnimationComponent& anim = player->Animation;

	if (anim.Has_full_anim_played)
	{
		set_player_state(player, Idle);
		change_player_animation(player, get_anim(0));
	}

	grounded_physics_update(player, dt);
}

void physics_ledge_balance_update(Player* player, float dt)
{
	grounded_physics_update(player, dt);
}

void physics_flip_on_input(Player* player, float dt)
{
	AnimationComponent& anim = player->Animation;
	float x_input = player->Input.Left_stick_x;

	if (abs(x_input) > 0.2f)
	{
		if (x_input > 0 && anim.Is_flipped)
		{
			set_player_state(player, TurnAround);
			change_player_animation(player, get_anim(5), LastFrameStick);
		}
		else if (x_input < 0 && !anim.Is_flipped)
		{
			set_player_state(player, TurnAround);
			change_player_animation(player, get_anim(5), LastFrameStick);
		}
	}
}

void physics_set_velocity_to_input(float x_input, float& x_velocity)
{
	if (abs(x_input) == 1.0f)
	{
		x_velocity = x_input;
	}
}

void physics_add_input_to_velocity(float x_input, float& x_velocity, float dt)
{
	x_velocity += x_input * dt;
}

void physics_apply_input_to_velocity(Player* player, float dt)
{
	InputComponent& input = player->Input;
	TransformComponent& transform = player->Transform;
	AnimationComponent& anim = player->Animation;
	PhysicsComponent& physics = player->Physics;
	ColliderComponent& collider = player->Collider;
	ActionStateComponent& state = player->ActionState;
	LocomotionComponent& loco = player->Locomotion;

	glm::vec2& v = physics.Velocity;

	// HACK ? maybe this is a hack
	if (state.Action_state == ActionState::TurnAround)
	{
		if (input.Left_stick_x != 0)
		{
			v.x += input.Left_stick_x * loco.Walk_speed * dt;
		}
	}
	else
	{
		if (input.Left_stick_x != 0)
		{
			float speed = state.Action_state == ActionState::Walking ? loco.Walk_speed : loco.Run_speed;

			if (input.Left_stick_x * v.x < speed * 0.5f)
			{
				v.x = input.Left_stick_x * speed;
			}

			v.x += input.Left_stick_x * speed * dt;
		}
	}

	physics_apply_drag(player, dt);
}

void physics_apply_drag(Player* player, float dt)
{
	PhysicsComponent& physics = player->Physics;

	glm::vec2& v = physics.Velocity;

	if (abs(v.x) >= 0.1f)
	{
		physics.Acceleration.x = -v.x * 5.0f;
		v.x += physics.Acceleration.x * dt;
	}
	else
	{
		physics.Acceleration.x = 0.0f;
		v.x = 0.0f;
	}
}

void physics_check_grab_ledge(Player* player, float dt)
{
	TransformComponent& transform = player->Transform;
	PhysicsComponent& physics = player->Physics;
	ActionStateComponent& state = player->ActionState;
	AnimationComponent& anim = player->Animation;

	glm::vec2& v = physics.Velocity;

	if (!player->Locomotion.Can_ledge_grab) { return; }
	//if (v.y > 0.0f) { return; } // TODO make this work, it currently sets player to be inside of level
	// probably because of some shit reason

	if (player->Input.Left_stick_y < -0.1f) { return; }

	glm::vec2 origin = glm::vec2(transform.Position.x, transform.Position.y + 16 * 0.02f);
	glm::vec2 direction = glm::vec2(anim.Is_flipped ? -1.0f : 1.0f, 0.0f);
	float distance = 0.25f;

	//render_quad(origin + (direction * distance), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(0.1f, 0.1f));
	RaycastHit hit = RaycastHit();
	if (raycast(origin, direction, distance, hit))
	{
		v = glm::vec2(0.0f);
		transform.Position = glm::vec2(hit.point.x + (anim.Is_flipped? 4 * 0.02f : -4 * 0.02f), hit.point.y - 16 * 0.02f);
		//render_quad(hit.point, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.01f, 0.01f));
		player->Locomotion.Can_double_jump = false;
		player->Locomotion.Can_ledge_grab = false;
		player->Locomotion.Current_ledge_grab_timer = 0.0f;

		vfx_spawn_effect(get_vfx_anim(0), hit.point, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), nullptr, nullptr, LastFrameStick);

		set_player_state(player, Ledgegrab);
		set_player_state(player, Special);
		change_player_animation(player, get_anim(7), LastFrameStick);
	}
}

void physics_land_on_touch_ground(Player* player)
{
	TransformComponent& transform = player->Transform;
	PhysicsComponent& physics = player->Physics;
	ActionStateComponent& state = player->ActionState;

	glm::vec2& v = physics.Velocity;

	if (v.y > 0.0f)
	{
		return;
	}

	RaycastHit hit = RaycastHit();
	if (raycast(transform.Position, glm::vec2(0.0f, -1.0f), 0.01f, hit))
	{
		// TODO knockdown on hit ground too hard (lay down, effects and whatnot) (check velocity on impact)
		if (player->Reset_time_scale_on_land)
		{
			reset_time_scale();
		}

		player->Locomotion.Can_double_jump = true;

		//printf("Player: %i hit ground with velocity.y: %f\n", player->ID, v.y);

		v.y = 0.0f;
		transform.Position.y = hit.point.y;

		if (state.Action_state == Knockback)
		{
			set_player_state(player, Grounded);
			set_player_state(player, Knockdown);
			change_player_animation(player, get_anim(6), LastFrameStick);
			return;
		}

		// TODO special case aerial attack l-cancel or summink
		if (state.Action_state != Attacking)
		{
			set_player_state(player, Grounded);
			set_player_state(player, Idle);
			return;

		} 
		else
		{
			set_player_state(player, Grounded);
		}
		// TODO landing lag if landed while attacking
	}
}
