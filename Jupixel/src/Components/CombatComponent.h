#pragma once

#include "Attack.h"

#include <vector>

struct CombatComponent
{
	Attack* Current_attack = nullptr;
	std::vector<Attack> Attacks = std::vector<Attack>();
	float Current_timer = 0.0f;
};
