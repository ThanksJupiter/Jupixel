#include "Player.h"

void change_player_animation(Player* player, Spritesheet* new_anim)
{
	player->Animation.Current_anim = new_anim;
	player->Animation.Current_Sprite_Index = 0;
}

void change_player_state(Player* player, int state)
{
	player->ActionState.Value = state;
	player->ActionState.Timer = 0.0f;
}

void change_player_state(Player* player, int state, float duration)
{
	player->ActionState.Value = state;
	player->ActionState.Timer = 0.0f;
	player->ActionState.Time_in_state = duration;
}
