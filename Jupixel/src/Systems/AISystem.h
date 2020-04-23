#pragma once

#include "Player.h"

void update_ai_system(Player& player, float dt);

void approach_player(Player& player, float dt);
void attack_close_range(Player& player, float dt);
void attack_medium_range(Player& player, float dt);
