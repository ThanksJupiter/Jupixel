#include "jppch.h"

#include "StateInitialization.h"

#include "States/Grounded/LocomotionStates.h"
#include "States/Position/PositionStates.h"
#include "States/Combat/CombatStates.h"

States create_states()
{
	States states;

	states.Idle = create_idle_state();
	states.Walk = create_walk_state();
	states.Run = create_run_state();

	states.Grounded = create_grounded_state();
	states.Airborne = create_airborne_State();
	states.Special = create_special_state();

	return states;
}
