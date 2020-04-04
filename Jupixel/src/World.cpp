#include "World.h"

#include "Player.h"

#include "Systems/InputSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/ActionStateSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/CameraController.h"
#include "Systems/MatchSystem.h"

#include "SkeletonAnimations.h"
#include "Renderer/Renderer.h"
#include "Texture2D.h"
#include "Animation/Sprite.h"
#include "UserInput.h"
#include "Key.h"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "ImGui/GUILayer.h"
#include "GLFW/glfw3.h"
#include "Physics/Raycaster.h"
#include "Components/LevelComponent.h"

World world = World();

Player* player_one = nullptr;
Player* player_two = nullptr;

Level level = Level();

void setup_world()
{
	world.player_one = new Player();
	world.player_two = new Player();

	setup_player(world.player_one, world.player_two, 0);
	setup_player(world.player_two, world.player_one, 1);

	player_one = world.player_one;
	player_two = world.player_two;

	player_one->Animation.Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	player_two->Animation.Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	level.Texture = *load_texture("assets/textures/Level.png");
	level.Sprite = Sprite(&level.Texture, 256, 22, 0);
	level.Scale = glm::vec2(level.Sprite.Width * 0.02f, level.Sprite.Height * 0.02f);
	level.Position = glm::vec2(0.0f, -1.17f);

	level.Collider.Scale = glm::vec2(level.Scale.x, level.Scale.y * 0.80f);
	level.Collider.Position = level.Position;

	add_target_collider(&level.Collider);
	set_world(&world);
}

void update_world(float dt)
{
	debug_functionality();

	update_input_system(player_one);
	update_input_system(player_two);

	update_action_state_system(player_one, dt);
	update_action_state_system(player_two, dt);

	update_physics_system(player_one, player_two, dt);

	test_collisions();

	update_animation_system(player_one, dt);
	update_animation_system(player_two, dt);	

	camera_update(player_one, player_two, dt);


	// HACK to render level without adding separate buffers
	update_player_animation(nullptr, &level.Sprite);
	render_quad(level.Texture, level.Position, level.Scale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	begin_GUI();
	update_match(dt);
	ImGui::Begin("Player stats");
	ImGui::Text("Player: %i\n pos state: %s\n action state: %s\n Health: %f",
		player_one->ID,
		get_position_state_name(player_one->ActionState.Position_state),
		get_action_state_name(player_one->ActionState.Action_state),
		player_one->Combat.Current_health_percentage);

	ImGui::Text("Player: %i\n pos state: %s\n action state: %s\n Health: %f",
		player_two->ID,
		get_position_state_name(player_two->ActionState.Position_state),
		get_action_state_name(player_two->ActionState.Action_state),
		player_two->Combat.Current_health_percentage);
	ImGui::End();
	end_GUI();
}

float get_time_scale()
{
	return world.current_time_scale;
}

void set_time_scale(float value)
{
	world.current_time_scale = value;
}

void reset_time_scale()
{
	world.current_time_scale = 1.0f;
}

void debug_functionality()
{
	if (is_key_pressed(KeyCode::R) || is_button_held(0, GLFW_GAMEPAD_BUTTON_B))
	{
		player_one->Combat.Current_health_percentage = 0.0f;
		player_two->Combat.Current_health_percentage = 0.0f;

		player_one->Transform.Position = glm::vec2(0.0f, 0.0f);
		player_two->Transform.Position = glm::vec2(0.0f, 0.0f);

		player_one->ActionState.Position_state = Airborne;
		player_one->ActionState.Action_state = Falling;

		player_two->ActionState.Position_state = Airborne;
		player_two->ActionState.Action_state = Falling;

		player_one->Physics.Velocity = glm::vec2(0.0f, 0.0f);
		player_two->Physics.Velocity = glm::vec2(0.0f, 0.0f);

		printf("Health reset!\n");
	}
}
