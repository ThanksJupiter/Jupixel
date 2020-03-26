#include "World.h"

#include "Player.h"

#include "Systems/InputSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/AnimationSystem.h"

#include "SkeletonAnimations.h"
#include "Renderer/Renderer.h"
#include "Systems/ActionStateSystem.h"
#include "Systems/CollisionSystem.h"

World* world = nullptr;

Player* player_one = nullptr;
Player* player_two = nullptr;

void setup_world()
{
	world = new World();

	world->player_one = new Player();
	world->player_two = new Player();

	player_one = world->player_one;
	player_two = world->player_two;

	player_two->ID = 1;

	player_one->Animation.Current_anim = get_idle_sheet();
	player_one->Animation.Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	player_two->Animation.Current_anim = get_idle_sheet();
	player_two->Animation.Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	player_one->Opponent = player_two;
	player_two->Opponent = player_one;
}

void update_world(float dt)
{
	update_input_system(player_one);
	update_input_system(player_two);

	update_action_state_system(player_one, dt);
	update_action_state_system(player_two, dt);

	update_position_system(player_one, dt);
	update_position_system(player_two, dt);

	test_collisions();

	update_animation_system(player_one, dt);
	update_animation_system(player_two, dt);	
}
