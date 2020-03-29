#pragma once

struct Player;

void update_action_state_system(Player* player, float dt);
void grounded_update(Player* player, float dt);
void airborne_update(Player* player, float dt);

void state_idle_update(Player* player, float dt);
void state_walk_update(Player* player, float dt);
void state_attack_update(Player* player, float dt);
void state_jump_update(Player* player, float dt);
void state_fall_update(Player* player, float dt);
