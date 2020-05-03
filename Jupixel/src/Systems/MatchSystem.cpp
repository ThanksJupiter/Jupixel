#include "jppch.h"

#include "MatchSystem.h"

#include "World.h"
#include "../ImGui/GUILayer.h"
#include "ActionStateSystem.h"
#include "imgui.h"
#include "VFXSystem.h"
#include "SkeletonAnimations.h"
#include "Renderer/Renderer.h"

MatchData match_data = MatchData();
World* current_world = nullptr;

glm::vec2 death_zone = glm::vec2(5.5f, 4.0f);

void set_world(World* world)
{
	current_world = world;
}

void begin_match()
{
	reset_match();
}

void update_match(float dt)
{
	handle_player_death(*current_world->player_one);
	handle_player_death(*current_world->player_two);

	display_match_GUI();
}

void display_match_GUI()
{
	ImGui::Begin("Match Data");
	ImGui::Text("Player: %i\n pos: %s\n stocks: %i",
				current_world->player_one->ID,
				glm::to_string(current_world->player_one->Transform.Position).c_str(),
				current_world->player_one->MatchData.Current_stock_count);

	ImGui::Text("Player: %i\n pos: %s\n stocks: %i",
				current_world->player_two->ID,
				glm::to_string(current_world->player_two->Transform.Position).c_str(),
				current_world->player_two->MatchData.Current_stock_count);
	ImGui::End();
}

void end_match()
{
	
}

void handle_player_death(Player& player)
{
	if (player_out_of_bounds(player))
	{
		player.MatchData.Current_stock_count--;

		vfx_spawn_effect(get_vfx_anim(3), player.Transform.Position + glm::vec2(0.0f, 0.3f));

		if (player.MatchData.Current_stock_count == 0)
		{
			printf("Player: %i dieded! :/", player.ID);
			reset_match();
			return;
		}

		respawn_player(player);
	}
}

void respawn_player(Player& player)
{
	player.Combat.Current_health_percentage = 0.0f;
	player.Transform.Position = glm::vec2(0.0f, 0.0f);
	player.ActionState.Position_state = PositionState::Airborne;
	player.ActionState.Action_state = ActionState::Falling;
	player.Physics.Velocity = glm::vec2(0.0f, 0.0f);
}

void reset_player(Player& player, glm::vec2& respawn_pos)
{
	respawn_player(player);
	player.Transform.Position = respawn_pos;
	player.MatchData.Current_stock_count = match_data.Stock_count;
}

void reset_match()
{
	match_data.Current_time = 0.0f;
	
	reset_player(*current_world->player_one, glm::vec2(1.0f, 0.0f));
	reset_player(*current_world->player_two, glm::vec2(-1.0f, 0.0f));
}

bool player_out_of_bounds(Player& player)
{
	render_quad_outline(glm::vec2(0.0f, 0.0f), death_zone * 2.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	return abs(player.Transform.Position.x) > death_zone.x || abs(player.Transform.Position.y) > death_zone.y;
}
