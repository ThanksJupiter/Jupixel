#include "jppch.h"

#include "PositionStates.h"

#include "states/StateThings_incl.h"

State create_grounded_state()
{
	State ret = State("Grounded");

	ret.Position_state = PositionState::Grounded;

	ret.enter = [](Player& p)
	{
		
	};

	ret.exit = [](Player& p)
	{
		
	};

	ret.update = [](Player& p, float dt)
	{
		InputComponent input = p.Input;
		TransformComponent& transform = p.Transform;
		AnimationComponent& anim = p.Animation;
		PhysicsComponent& physics = p.Physics;
		ColliderComponent& collider = p.Collider;
		ActionStateComponent& state = p.ActionState;
		CombatComponent& combat = p.Combat;
		LocomotionComponent& loco = p.Locomotion;

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
			if (!p.is_facing_travel_direction())
			{
				if (p.is_inputting_in_travel_direction())
				{
					v.x = input.Left_stick_x;
				}

				//set_player_state(player, PositionState::Airborne);
				//set_player_state(player, ActionState::Falling);
			}

			if (state.Action_state == ActionState::Walking && abs(input.Left_stick_x) < loco.Ledge_balance_threshold)
			{
				transform.Position.x -= v.x * dt;
				v.x = 0.0f;

				//set_player_state(player, ActionState::LedgeBalance);
				//change_player_animation(player, get_anim(10), Loop);
				return;
			}

			if (state.Action_state != ActionState::Attacking || !loco.Can_airdodge)
			{
				//set_player_state(player, PositionState::Airborne);
				//set_player_state(player, ActionState::Falling);
				return;
			}
			else
			{
				transform.Position.x -= v.x * dt;
				v.x = 0.0f;
				loco.Ledge_balance_queued = true;
			}
		}
	};

	return ret;
}

State create_airborne_State()
{
	State ret = State("Airborne");

	ret.Position_state = PositionState::Airborne;

	ret.enter = [](Player& p)
	{
		
	};

	ret.exit = [](Player& p)
	{
		
	};

	ret.update = [](Player& p, float dt)
	{
		InputComponent& input = p.Input;
		TransformComponent& transform = p.Transform;
		AnimationComponent& anim = p.Animation;
		PhysicsComponent& physics = p.Physics;
		ColliderComponent& collider = p.Collider;
		ActionStateComponent& state = p.ActionState;
		CombatComponent& combat = p.Combat;

		if (state.Action_state != ActionState::Ledgegrab)
		{
			p.Locomotion.Current_ledge_grab_timer += dt;

			if (p.Locomotion.Current_ledge_grab_timer >= p.Locomotion.Ledge_grab_time)
			{
				p.Locomotion.Current_ledge_grab_timer = 0.0f;
				p.Locomotion.Can_ledge_grab = true;
			}
		}

		glm::vec2& v = physics.Velocity;

		if (input.Left_stick_x != 0 && p.Locomotion.Has_aerial_control)
		{
			if (p.is_inputting_in_travel_direction())
			{
				if (abs(v.x) < 1.5f)
				{
					phys_add_input_to_velocity(input.Left_stick_x * 2.0f, v.x, dt);
				}
			}
			else
			{
				phys_add_input_to_velocity(input.Left_stick_x * 7.0f, v.x, dt);
			}
			//v.x = input.Left_stick_x * 1.0f;
		}

		if (state.Action_state != ActionState::Attacking)
		{
			phys_check_grab_ledge(p, dt);
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

		float fall_add = 10.0f;
		if (v.y < 1.0f)
		{
			if (v.y < 0.0f && v.y > -1.0f)
			{
				fall_add = input.Left_stick_y < 0.0f ? -input.Left_stick_y * 60.0f : 10.0f;
			}
			else
			{
				fall_add = input.Left_stick_y < 0.0f ? -input.Left_stick_y * 15.0f : 10.0f;
			}
		}

		phys_land_on_touch_ground(p);
		v.y -= fall_add * dt;
	};

	return ret;
}

State create_special_state()
{
	State ret = State("Special");

	ret.Position_state = PositionState::Special;

	ret.enter = [](Player& p)
	{
		
	};

	ret.exit = [](Player& p)
	{
		
	};

	ret.update = [](Player& p, float dt)
	{
		
	};

	return ret;
}
