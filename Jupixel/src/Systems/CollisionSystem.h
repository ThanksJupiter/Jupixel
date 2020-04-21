#pragma once
#include "Player.h"

struct CollisionTestRequest;

void queue_collision(CollisionTestRequest request);
void test_collisions();
void resolve_collisions(Player& player);
