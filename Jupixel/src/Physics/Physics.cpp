#include "jppch.h"

#include "Physics.h"

#include "Components/Components_incl.h"
#include "Raycaster.h"
#include "Systems/VFXSystem.h"
#include "SkeletonAnimations.h"

void phys_flip_on_input(Player& player, float dt)
{
	if (!player.Locomotion.Can_airdodge) { return; }

	AnimationComponent& anim = player.Animation;
	float x_input = player.Input.Left_stick_x;

	if (abs(x_input) > 0.2f)
	{
		if (x_input > 0 && anim.Is_flipped)
		{
			// TODO enter_turnaround_state(duration);
			//set_player_state(player, ActionState::TurnAround);
			//change_player_animation(player, get_anim(5), LastFrameStick);
		}
		else if (x_input < 0 && !anim.Is_flipped)
		{
			//set_player_state(player, ActionState::TurnAround);
			//change_player_animation(player, get_anim(5), LastFrameStick);
		}
	}
}

void phys_set_velocity_to_input(float x_input, float& x_velocity)
{
	if (abs(x_input) == 1.0f)
	{
		x_velocity = x_input;
	}
}

void phys_add_input_to_velocity(float x_input, float& x_velocity, float dt)
{
	x_velocity += x_input * dt;
}

void phys_apply_input_to_velocity(Player& player, float dt)
{
	InputComponent& input = player.Input;
	TransformComponent& transform = player.Transform;
	AnimationComponent& anim = player.Animation;
	PhysicsComponent& physics = player.Physics;
	ColliderComponent& collider = player.Collider;
	ActionStateComponent& state = player.ActionState;
	LocomotionComponent& loco = player.Locomotion;

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

	//physics_apply_drag(player, dt);
}

void phys_apply_drag(Player& player, float dt)
{
	PhysicsComponent& physics = player.Physics;

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

void phys_check_grab_ledge(Player& player, float dt)
{
	TransformComponent& transform = player.Transform;
	PhysicsComponent& physics = player.Physics;
	ActionStateComponent& state = player.ActionState;
	AnimationComponent& anim = player.Animation;

	glm::vec2& v = physics.Velocity;

	if (!player.Locomotion.Can_ledge_grab) { return; }
	//if (v.y > 0.0f) { return; } // TODO make this work, it currently sets player to be inside of level
	// probably because of some shit reason

	if (player.Input.Left_stick_y < -0.1f) { return; }

	glm::vec2 origin = glm::vec2(transform.Position.x, transform.Position.y + 16 * 0.02f);
	glm::vec2 direction = glm::vec2(anim.Is_flipped ? -1.0f : 1.0f, 0.0f);
	float distance = 0.25f;

	//render_quad(origin + (direction * distance), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(0.1f, 0.1f));
	RaycastHit hit = RaycastHit();
	if (raycast(origin, direction, distance, hit))
	{
		v = glm::vec2(0.0f);
		transform.Position = glm::vec2(hit.point.x + (anim.Is_flipped ? 4 * 0.02f : -4 * 0.02f), hit.point.y - 16 * 0.02f);
		//render_quad(hit.point, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.01f, 0.01f));
		player.Locomotion.Can_airdodge = true;
		player.Locomotion.Can_double_jump = false;
		player.Locomotion.Can_ledge_grab = false;
		player.Locomotion.Current_ledge_grab_timer = 0.0f;

		vfx_spawn_effect(get_vfx_anim(0), hit.point, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), nullptr, nullptr, LastFrameStick);

		//set_player_state(player, ActionState::Ledgegrab);
		//set_player_state(player, PositionState::Special);
		//change_player_animation(player, get_anim(7), LastFrameStick);
	}
}

void phys_land_on_touch_ground(Player& player)
{
	TransformComponent& transform = player.Transform;
	PhysicsComponent& physics = player.Physics;
	ActionStateComponent& state = player.ActionState;

	glm::vec2& v = physics.Velocity;

	if (v.y > 0.0f)
	{
		return;
	}

	RaycastHit hit = RaycastHit();
	if (raycast(transform.Position, glm::vec2(0.0f, -1.0f), 0.01f, hit))
	{
		// TODO knockdown on hit ground too hard (lay down, effects and whatnot) (check velocity on impact)
		if (player.Reset_time_scale_on_land)
		{
			//reset_time_scale();
		}

		player.Locomotion.Can_double_jump = true;

		//printf("Player: %i hit ground with velocity.y: %f\n", player.ID, v.y);

		v.y = 0.0f;
		transform.Position.y = hit.point.y;

		if (state.Action_state == ActionState::Airdodge)
		{
			//set_player_state(player, PositionState::Special);
			//set_player_state(player, ActionState::Idle);
			return;
		}

		player.Locomotion.Can_airdodge = true;

		player.set_position_state(get_states().Grounded);
		player.set_locomotion_state(get_states().Idle);

		//set_player_state(player, PositionState::Grounded);
		//set_player_state(player, ActionState::Idle);

		if (state.Action_state == ActionState::Knockback)
		{
			//set_player_state(player, ActionState::Knockdown);
			//change_player_animation(player, get_anim(6), LastFrameStick);
		}
	}
}

