#pragma once

#include "States/State.h"
#include "Player.h"

State create_grounded_attack_state();
State create_airborne_attack_state();

void combat_perform_attack(Player& p, float dt);
