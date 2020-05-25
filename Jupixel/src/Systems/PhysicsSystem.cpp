#include "jppch.h"

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

#include "Renderer/Renderer.h"
#include "VFXSystem.h"

const float knockback_scale_factor = 0.015f;

void update_physics_system(Player* player, float dt)
{
	ActionStateComponent& state = player->ActionState;

	if (get_time_scale() < 1.0f)
	{
		set_time_scale(get_time_scale() + 60.0f * dt);
	}
	else
	{
		reset_time_scale();
	}

	player->Locomotion.Has_aerial_control = player->Locomotion.Can_airdodge;

	switch (state.Action_state)
	{
		case ActionState::Idle:
			physics_idle_update(player, dt);
			break;
		case ActionState::Walking:
			physics_walk_update(player, dt);
			break;
		case ActionState::Running:
			physics_run_update(player, dt);
			break;
		case ActionState::Attacking:
			physics_attack_update(player, dt);
			break;
		case ActionState::Jumping:
			physics_jump_update(player, dt);
			break;
		case ActionState::Falling:
			physics_fall_update(player, dt);
			break;
		case ActionState::Crouching:
			physics_crouch_update(player, dt);
			break;
		case ActionState::Airdodge:
			physics_airdodge_update(player, dt);
			break;
		case ActionState::Knockback:
			physics_knockback_update(player, dt);
			break;
		case ActionState::None:
			break;
		default:
			printf("invalid action state");
			break;
		case ActionState::Locomotion:
			break;
		case ActionState::TurnAround:
			physics_turn_around_update(player, dt);
			break;
		case ActionState::JumpSquat:
			break;
		case ActionState::Knockdown:
			physics_knockdown_update(player, dt);
			break;
		case ActionState::Ledgegrab:
			
			break;
		case ActionState::Getup:
			physics_getup_update(player, dt);
			break;
		case ActionState::LedgeBalance:
			physics_ledge_balance_update(player, dt);
			break;
		case ActionState::Block:
			physics_block_update(player, dt);
			break;
		case ActionState::Locomoting:
			break;
		case ActionState::Reaching:
			physics_reach_update(player, dt);
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
	if (state.Action_state == ActionState::Attacking && !combat.Allow_attacking_movement)
	{
		v.x = 0.0f;
	}

	RaycastHit hit = RaycastHit();
	if (raycast(transform.Position, glm::vec2(0.0f, -1.0f), 0.1f, hit))
	{
		transform.Position.x += v.x * dt;
	}
	else // TODO know if *is about* to walk off edge
	{
		if (!player->is_facing_travel_direction())
		{
			if (player->is_inputting_in_travel_direction())
			{
				v.x = input.Left_stick_x;
			}

			set_player_state(player, PositionState::Airborne);
			set_player_state(player, ActionState::Falling);
		}

		if (state.Action_state == ActionState::Walking && abs(input.Left_stick_x) < loco.Ledge_balance_threshold)
		{
			transform.Position.x -= v.x * dt;
			v.x = 0.0f;

			set_player_state(player, ActionState::LedgeBalance);
			change_player_animation(player, get_anim(10), Loop);
			return;
		}

		if (state.Action_state != ActionState::Attacking || !loco.Can_airdodge)
		{
			set_player_state(player, PositionState::Airborne);
			set_player_state(player, ActionState::Falling);
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

	if (state.Action_state != ActionState::Ledgegrab)
	{
		player->Locomotion.Current_ledge_grab_timer += dt;

		if (player->Locomotion.Current_ledge_grab_timer >= player->Locomotion.Ledge_grab_time)
		{
			player->Locomotion.Current_ledge_grab_timer = 0.0f;
			player->Locomotion.Can_ledge_grab = true;
		}
	}

	glm::vec2& v = physics.Velocity;

	if (input.Left_stick_x != 0 && player->Locomotion.Has_aerial_control)
	{
		if (player->is_inputting_in_travel_direction())
		{
			if (abs(v.x) < 1.5f)
			{
				physics_add_input_to_velocity(input.Left_stick_x * 6.0f, v.x, dt);
			}
		}
		else
		{
			physics_add_input_to_velocity(input.Left_stick_x * 7.0f, v.x, dt);
		}
		//v.x = input.Left_stick_x * 1.0f;
	}

	if (state.Action_state != ActionState::Attacking)
	{
		physics_check_grab_ledge(player, dt);
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

	float fall_add = 40.0f;
	if (v.y < 1.0f)
	{
		// fast falling
		if (v.y < 0.0f && v.y > -1.0f) // only fast fall within apex of jump
		{
			// impact fall speed more if within apex
			fall_add = input.Left_stick_y < 0.0f ? -input.Left_stick_y * 60.0f : 10.0f;
		}
		else
		{
			fall_add = input.Left_stick_y < 0.0f ? -input.Left_stick_y * 15.0f : 10.0f;
		}
	}

	v.y -= fall_add * dt;
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

	if (state.Action_state == ActionState::Ledgegrab) { return; }

	InputComponent input = player->Input;
	LocomotionComponent& loco = player->Locomotion;

	if (!player->is_grounded())
	{
		if (player->is_facing_travel_direction())
		{
			transform.Position.x -= v.x * dt;
			v.x = 0.0f;

			set_player_state(player, ActionState::LedgeBalance);
			set_player_state(player, PositionState::Grounded);
			change_player_animation(player, get_anim(10), Loop);
			return;
		}
		else
		{
			v.x = v.x * 0.3f;
			set_player_state(player, ActionState::Falling);
			set_player_state(player, PositionState::Airborne);
		}
	}

	if (!player->Locomotion.Can_airdodge)
	{
		grounded_physics_update(player, dt);
	}
	else if (state.Action_state != ActionState::JumpSquat)
	{
		set_player_state(player, PositionState::Grounded);
		set_player_state(player, ActionState::Idle);
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
		case PositionState::Grounded:

			physics_add_input_to_velocity(input.Left_stick_x, v.x, dt);
			physics_flip_on_input(player, dt);
			physics_apply_drag(player, dt);
			grounded_physics_update(player, dt);

			break;
		case PositionState::Airborne:
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
		case PositionState::Grounded:

			//physics_add_velocity_to_input(input.Left_stick_x * loco.Run_speed, v.x, dt);
			physics_apply_input_to_velocity(player, dt);
			physics_flip_on_input(player, dt);
			grounded_physics_update(player, dt);

			break;
		case PositionState::Airborne:
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
		case PositionState::Grounded:

			physics_set_velocity_to_input(input.Left_stick_x * loco.Run_speed, v.x);
			physics_apply_input_to_velocity(player, dt);
			//physics_flip_on_input(player, dt);
			grounded_physics_update(player, dt);

			// TODO ground physics update before state updates?

			break;
		case PositionState::Airborne:
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
		set_player_state(player, ActionState::Idle);
	}

	//physics_add_velocity_to_input(player->Input.Left_stick_x, v.x, dt);
	physics_apply_input_to_velocity(player, dt);
	grounded_physics_update(player, dt);
}

void physics_attack_update(Player* p, float dt)
{
	ActionStateComponent& state = p->ActionState;
	CombatComponent& combat = p->Combat;
	AnimationComponent& anim = p->Animation;
	InputComponent input = p->Input;

	combat.Current_timer += dt;

	if (anim.Current_Sprite_Index >= combat.Current_attack->Hitbox_start_frame &&
		anim.Current_Sprite_Index <= combat.Current_attack->Hitbox_stop_frame &&
		!combat.Is_current_attack_resolved)
	{
		CollisionTestRequest request = CollisionTestRequest();
		AnimationComponent anim = p->Animation;

		request.Instigator = p;
		request.Target = p->Opponent;

		ColliderComponent col_comp = p->Combat.Current_attack->Hitbox;
		request.Collider = col_comp;

		request.Collider.Position = p->Transform.Position;
		request.Collider.Offset.x = col_comp.Offset.x * (p->Animation.Is_flipped ? -1 : 1);

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
			if (p->Locomotion.Ledge_balance_queued && input.Left_stick_x == 0.0f)
			{
				set_player_state(p, ActionState::LedgeBalance);
				change_player_animation(p, get_anim(10), Loop);
				p->Locomotion.Ledge_balance_queued = false;
			}
			else
			{
				set_player_state(p, ActionState::Idle);
				p->Locomotion.Ledge_balance_queued = false;
			}
		}
		else
		{
			set_player_state(p, ActionState::Falling);
		}
	}

	switch (state.Position_state)
	{
		case PositionState::Grounded:
			grounded_physics_update(p, dt);
			break;
		case PositionState::Airborne:
			physics_land_on_touch_ground(p);
			airborne_physics_update(p, dt);
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
		case PositionState::Grounded:
			printf("grounded and jumping, this seems wroong :D");
			break;
		case PositionState::Airborne:

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
		case PositionState::Grounded:
			printf("grounded and falling, this seems wroong :D");
			break;
		case PositionState::Airborne:
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
		case PositionState::Grounded:

			grounded_physics_update(player, dt);
			break;
		case PositionState::Airborne:

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
		case PositionState::Grounded:
			grounded_physics_update(player, dt);
			break;
		case PositionState::Airborne:

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
		set_player_state(player, ActionState::Getup);
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
		set_player_state(player, ActionState::Idle);
		change_player_animation(player, get_anim(0));
	}

	grounded_physics_update(player, dt);
}

void physics_ledge_balance_update(Player* player, float dt)
{
	grounded_physics_update(player, dt);
}

void physics_block_update(Player* player, float dt)
{
	physics_apply_drag(player, dt);
	grounded_physics_update(player, dt);
}

void physics_airdodge_update(Player* player, float dt)
{
	physics_apply_drag(player, dt);

	airborne_physics_update(player, dt);
	physics_land_on_touch_ground(player);
}

void physics_reach_update(Player* player, float dt)
{
	grounded_physics_update(player, dt);
}

void physics_flip_on_input(Player* player, float dt)
{
	if (!player->Locomotion.Can_airdodge) { return; }

	AnimationComponent& anim = player->Animation;
	float x_input = player->Input.Left_stick_x;

	if (abs(x_input) > 0.2f)
	{
		if (x_input > 0 && anim.Is_flipped)
		{
			set_player_state(player, ActionState::TurnAround);
			change_player_animation(player, get_anim(5), LastFrameStick);
		}
		else if (x_input < 0 && !anim.Is_flipped)
		{
			set_player_state(player, ActionState::TurnAround);
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

			if (state.Action_state != ActionState::JumpSquat)
			{
				v.x += input.Left_stick_x * speed * dt;
			}
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
		player->Locomotion.Can_airdodge = true;
		player->Locomotion.Can_double_jump = false;
		player->Locomotion.Can_ledge_grab = false;
		player->Locomotion.Current_ledge_grab_timer = 0.0f;

		vfx_spawn_effect(get_vfx_anim(0), hit.point, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), nullptr, nullptr, LastFrameStick);

		set_player_state(player, ActionState::Ledgegrab);
		set_player_state(player, PositionState::Special);
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

		if (state.Action_state == ActionState::Airdodge)
		{
			set_player_state(player, PositionState::Special);
			set_player_state(player, ActionState::Idle);
			return;
		}

		player->Locomotion.Can_airdodge = true;

		set_player_state(player, PositionState::Grounded);
		set_player_state(player, ActionState::Idle);

		if (state.Action_state == ActionState::Knockback)
		{
			set_player_state(player, ActionState::Knockdown);
			change_player_animation(player, get_anim(6), LastFrameStick);
		}
	}
}
