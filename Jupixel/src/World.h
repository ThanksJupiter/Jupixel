#pragma once

struct Player;

struct World
{
	Player* player_one = nullptr;
	Player* player_two = nullptr;


};

void setup_world();
void update_world(float dt);
