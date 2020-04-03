#pragma once

#include "Attack.h"

#include <vector>

struct CombatComponent
{
	Attack* Current_attack = nullptr;
	std::vector<Attack> Attacks = std::vector<Attack>();
	float Current_timer = 0.0f;
	bool Is_current_attack_resolved = false;
	float Current_health_percentage = 0.0f;
	bool Allow_attacking_movement = false;
};
