#pragma once
#include <string>

struct Player;

void update_action_state_system(Player* player, float dt);

void state_grounded_update(Player* player, float dt);
void state_airborne_update(Player* player, float dt);

void state_idle_update(Player* player, float dt);
void state_walk_update(Player* player, float dt);
void state_run_update(Player* player, float dt);
void state_jump_squat_update(Player* player, float dt);
void state_attack_update(Player* player, float dt);
void state_jump_update(Player* player, float dt);
void state_fall_update(Player* player, float dt);
void state_ledgegrab_update(Player* player, float dt);
void state_ledge_balance_update(Player* player, float dt);

const char* get_position_state_name(int id);
const char* get_action_state_name(int id);
