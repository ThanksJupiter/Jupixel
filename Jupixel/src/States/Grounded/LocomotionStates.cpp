#include "jppch.h"

#include "LocomotionStates.h"

#include "States/StateThings_incl.h"

State create_idle_state()
{
	State ret = State("Idle");

	ret.Action_state = ActionState::Idle;
	ret.Position_state = PositionState::Grounded;
	ret.Locomotion_state = LocomotionState::Stationary;

	ret.enter = [](Player& p)
	{
		
	};

	ret.exit = [](Player& p)
	{
		
	};

	ret.update = [](Player& p, float dt)
	{
		InputComponent input = p.Input;
		CombatComponent& combat = p.Combat;
		LocomotionComponent& loco = p.Locomotion;

		if (abs(input.Left_stick_x == 0.0f))
		{
			loco.Current_dash_timer = 0.0f;
		}
		else if (p.is_inputting_in_travel_direction())
		{
			loco.Current_dash_timer += dt;
		}

		if (input.Attack)
		{
			p.set_position_state(get_states().Grounded_attack);
		}
	};

	return ret;
}

State create_walk_state()
{
	State ret = State("Walk");

	ret.Action_state = ActionState::Locomoting;
	ret.Position_state = PositionState::Grounded;
	ret.Locomotion_state = LocomotionState::Walking;

	ret.enter = [](Player& p)
	{
		
	};

	ret.exit = [](Player& p)
	{
		
	};

	ret.update = [](Player& p, float dt)
	{
		InputComponent input = p.Input;

		if (input.Attack)
		{
			p.set_position_state(get_states().Grounded_attack);
		}
	};

	return ret;
}

State create_run_state()
{
	State ret = State("Run");

	ret.Action_state = ActionState::Locomoting;
	ret.Position_state = PositionState::Grounded;
	ret.Locomotion_state = LocomotionState::Running;

	ret.enter = [](Player& p)
	{
		
	};

	ret.exit = [](Player& p)
	{
		
	};

	ret.update = [](Player& p, float dt)
	{
		InputComponent input = p.Input;
		CombatComponent& combat = p.Combat;

		if (input.Attack)
		{
			//p.perform_grounded_attack(combat.Attacks)
			//p.set_position_state(get_states().Grounded_attack);
		}
	};

	return ret;
}
