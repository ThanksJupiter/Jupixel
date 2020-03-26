#pragma once

struct Player;

enum PositionState { Grounded, Airborne };
enum ActionState { Idle, Walking, Attacking, Jumping, Falling };

void update_action_state_system(Player* player, float dt);
