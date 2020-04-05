#include "Player.h"

#include "SkeletonAnimations.h"
#include <stdio.h>
#include "Attack.h"

int hitbox_frames[] = 
{ 
	6, // fsmash
	2, // nair
	3, // uair
	4, // dair
	4, // fair
	3, // bair
	2, // dash tackle
	5, // usmash
	8, // dsmash
	2, // ftilt
	4  // dtilt
};

float attack_damage[] = 
{
	16.0f, // fsmash
	4.0f,  // nair
	6.0f,  // uair
	11.0f, // dair
	13.0f, // fair
	7.0f,  // bair
	6.0f,  // dash tackle
	18.0f, // usmash
	21.0f, // dsmash
	6.0f,  // ftilt
	3.0f   // dtilt
};

glm::vec2 knockback_vectors[] = 
{
	{ 2.0f, 1.6f },  // fsmash
	{ 1.2f, 1.2f },	 // nair
	{ 0.1f, 2.0f },	 // uair
	{ 0.0f, -2.0f }, // dair
	{ 1.3f, 1.5f },	 // fair
	{ 1.6f, 1.7 },	 // bair
	{ 1.8f, 1.0f },	 // dash tackle
	{ 0.3f, 3.0f },	 // usmash
	{ 0.7f, -3.0 },	 // dsmash
	{ 1.1f, 0.8f },	 // ftilt
	{ 0.2f, 1.7f }	 // dtilt
};

glm::vec2 hitbox_offsets[] =
{
	{ 0.35f, 0.2f },    // fsmash
	{ 0.18f, 0.1f },	// nair
	{ 0.0f, 0.4f},		// uair
	{ 0.0f, 0.05f },	// dair
	{ 0.15f, 0.25f },	// fair
	{ -0.25f, 0.2f },	// bair
	{ 0.18f, 0.2f },	// dash tackle
	{ 0.1f, 0.4f },		// usmash
	{ 0.18f, 0.0f },	// dsmash
	{ 0.18f, 0.1f },	// ftilt
	{ 0.42f, 0.04f }	// dtilt
};

glm::vec2 hitbox_scales[] =
{
	{ 0.4f, 0.2f },  // fsmash
	{ 0.15f, 0.15f },// nair
	{ 0.3f, 0.3f },	 // uair
	{ 0.25f, 0.2f }, // dair
	{ 0.3f, 0.2f },	 // fair
	{ 0.3f, 0.1f },	 // bair
	{ 0.2f, 0.2f },	 // dash tackle
	{ 0.3f, 0.3f },	 // usmash
	{ 0.1f, 0.1f },	 // dsmash
	{ 0.15f, 0.15f },// ftilt
	{ 0.18f, 0.1f }	 // dtilt
};

void setup_player(Player* player_to_setup, Player* player_opponent, int player_id)
{
	player_to_setup->ID = player_id;
	player_to_setup->Opponent = player_opponent;

	player_to_setup->Animation.Current_anim = get_anim(0);
	for (int i = 0; i < get_attack_anim_count(); i++)
	{
		Spritesheet sheet = *get_attack_anim(i);

		player_to_setup->Combat.Attacks.push_back(Attack(
			&sheet,
			hitbox_frames[i],
			knockback_vectors[i],
			hitbox_offsets[i],
			hitbox_scales[i],
			sheet.Frame_delay,
			attack_damage[i]));
	}
}

void change_player_animation(Player* player, Spritesheet* new_anim)
{
	player->Animation.Current_anim = new_anim;
	player->Animation.Current_Sprite_Index = 0;
}

void change_player_animation(Player* player, Spritesheet* new_anim, AnimationState state)
{
	player->Animation.Has_full_anim_played = false;
	player->Animation.Current_anim = new_anim;
	player->Animation.Current_Sprite_Index = 0;
	player->Animation.Anim_state = state;
}

void set_player_state(Player* player, PositionState state)
{
	player->ActionState.Position_state = state;
}

void set_player_state(Player* player, ActionState state)
{
	if (state <2)
	{
		change_player_animation(player, get_anim(state), Loop);
	}

	if (state == Falling)
	{
		change_player_animation(player, get_anim(8), Loop);
	}

	if (state == Attacking)
	{
		player->Combat.Is_current_attack_resolved = false;
	}

	player->ActionState.Previous_action_state = player->ActionState.Action_state;
	player->ActionState.Action_state = state;
	//printf("Player: %i entered state: %i\n", player->ID, state);
}
