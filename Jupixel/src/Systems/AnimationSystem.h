#pragma once

struct Player;

struct Sprite;

void update_animation_system(Player* player, float dt);
void update_player_animation(Player* player, Sprite* new_sprite);