#pragma once

#include "States/State.h"
#include "Player.h"
#include "States/PlayerStates.h"

void state_initialize(Player& player);
void state_update(Player& player, float dt);
