#pragma once

#include "State.h"

struct States
{
	States() {}

	State Grounded;
	State Airborne;
	State Special;

	State Idle;
	State Walk;
	State Run;
	
	State Grounded_attack;
	State Airborne_attack;
};

States get_states();
