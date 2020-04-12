#pragma once

struct Player;

struct World
{
	Player* player_one = nullptr;
	Player* player_two = nullptr;

	float current_time_scale = 1.0f;
};

void setup_world();
void update_world(float dt, float fixed_dt);

World* get_world();

float get_time_scale();
void set_time_scale(float value);
void reset_time_scale();
void debug_functionality();