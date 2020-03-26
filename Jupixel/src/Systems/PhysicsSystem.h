#pragma once

struct Player;

void update_position_system(Player* player, float dt);
void update_position_system(Player* player_one, Player* player_two, float dt);
