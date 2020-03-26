#include "Player.h"

#include "SkeletonAnimations.h"

void setup_player(Player* player_to_setup, Player* player_opponent, int player_id)
{
	player_to_setup->ID = player_id;
	player_to_setup->Opponent = player_opponent;

	player_to_setup->Animation.Current_anim = get_idle_sheet();
}

void change_player_animation(Player* player, Spritesheet* new_anim)
{
	player->Animation.Current_anim = new_anim;
	player->Animation.Current_Sprite_Index = 0;
}

void set_player_state(Player* player, int state)
{
	player->ActionState.Value = state;
	player->ActionState.Timer = 0.0f;
}

void set_player_state(Player* player, int state, float duration)
{
	player->ActionState.Value = state;
	player->ActionState.Timer = 0.0f;
	player->ActionState.Time_in_state = duration;
}

void set_player_state(Player* player, PositionState state)
{
	player->ActionState.Position_state = state;
}

void set_player_state(Player* player, ActionState state)
{
	player->ActionState.Action_state = state;
}
