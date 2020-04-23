#pragma once

#include "Attack.h"

#include <vector>

enum class BlockType
{
	High, Low, None
};

enum class AttackType
{
	High, Low, None
};

struct CombatComponent
{
	Attack* Current_attack = nullptr;
	std::vector<Attack> Attacks = std::vector<Attack>();
	float Current_timer = 0.0f;
	bool Is_current_attack_resolved = false;
	float Current_health_percentage = 0.0f;
	bool Allow_attacking_movement = false;
	BlockType Block_type = BlockType::High;
	AttackType Attack_type = AttackType::None;
	const float Knockback_scale_factor = 0.025f;
};
