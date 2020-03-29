#include "Player.h"

#include "SkeletonAnimations.h"
#include <stdio.h>
#include "Attack.h"

int hitbox_frames[] = { 6, 2, 3, 4, 4, 3 };

glm::vec2 knockback_vectors[] = 
{
	{ 2.0f, 1.6f },
	{ 0.8, 1.2f },
	{ 0.1f, 3.0f },
	{ 0.0f, 3.0f },
	{ 1.3f, 1.5f },
	{ 1.6f, 1.7 }
};

glm::vec2 hitbox_offsets[] =
{
	{ 0.35f, 0.2f },
	{ 0.25f, 0.2f },
	{ 0.0f, 0.4f},
	{ 0.0f, -0.0f },
	{ 0.2f, 0.2f },
	{ -0.25f, 0.2f }
};

glm::vec2 hitbox_scales[] =
{
	{ 0.4f, 0.2f },
	{ 0.2f, 0.2f },
	{ 0.3f, 0.3f },
	{ 0.45f, 0.2f },
	{ 0.3f, 0.3f },
	{ 0.3f, 0.1f }
};

void setup_player(Player* player_to_setup, Player* player_opponent, int player_id)
{
	player_to_setup->ID = player_id;
	player_to_setup->Opponent = player_opponent;

	player_to_setup->Animation.Current_anim = get_idle_sheet();

	for (int i = 0; i < get_attack_anim_count(); i++)
	{
		Spritesheet sheet = *get_attack_anim(i);

		player_to_setup->Combat.Attacks.push_back(Attack(
			&sheet,
			hitbox_frames[i],
			knockback_vectors[i],
			hitbox_offsets[i],
			hitbox_scales[i],
			sheet.Frame_delay));
	}
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
	if (state <2)
	{
		change_player_animation(player, get_anim(state));
	}

	player->ActionState.Action_state = state;
	printf("Player: %i entered state: %i\n", player->ID, state);
}
