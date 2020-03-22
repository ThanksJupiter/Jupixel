#include "World.h"

#include "Player.h"

#include "Systems/InputSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/AnimationSystem.h"

#include "SkeletonAnimations.h"
#include "Renderer/Renderer.h"

World* world = nullptr;

Player* player_one = nullptr;
Player* player_two = nullptr;

void setup_world()
{
	world = new World();

	world->player_one = new Player();
	world->player_two = new Player();

	player_one = world->player_one;
	player_one = world->player_two;

	player_one->animation.current_anim = get_idle_sheet();
	player_one->animation.Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

void update_world(float dt)
{
	update_input_system(player_one);
	//update_input_system(player_two);

	update_position_system(player_one, dt);
	//update_position_system(player_two, dt);

	begin_scene(1);
	update_animation_system(player_one, dt);
	//update_animation_system(player_two, dt);

	
}
