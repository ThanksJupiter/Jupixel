#pragma once

#include "Player.h"

#include <stdio.h>

struct AIGoal
{
	bool (*is_goal_completed)(Player&, Player&) = [](Player& ai_agent, Player& target) 
	{
		printf(
		"Default goal used for ai agent id: %i, have you not set one? ;)", ai_agent.ID);
		return false; 
	};
};
