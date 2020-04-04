#pragma once

#include "Player.h"

struct World;

struct MatchData
{
	float Current_time = 0.0f;
	float Time_limit = 880.0f;

	int Stock_count = 4;
};

void set_world(World* world);

void begin_match();
void update_match(float dt);
void display_match_GUI();
void end_match();

void handle_player_death(Player& player);
void respawn_player(Player& player);
void reset_player(Player& player, glm::vec2& respawn_pos);
void reset_match();

bool player_out_of_bounds(Player& player);
