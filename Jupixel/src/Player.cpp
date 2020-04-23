#include "Player.h"

#include "SkeletonAnimations.h"
#include <stdio.h>
#include "Attack.h"
#include "Physics/Raycaster.h"
#include "Systems/ActionStateSystem.h"
 
// TODO load attacks like states
int hitbox_start_frames[] = 
{ 
	6, // fsmash
	2, // nair
	3, // uair
	4, // dair
	4, // fair
	2, // bair
	2, // dash tackle
	5, // usmash
	8, // dsmash
	2, // ftilt
	4, // dtilt
	5, // utilt
	2, // first jab
	4 // second jab
};

int hitbox_stop_frames[] =
{
	7, // fsmash
	4, // nair
	6, // uair
	6, // dair
	6, // fair
	6, // bair
	4, // dash tackle
	7, // usmash
	9, // dsmash
	3, // ftilt
	5, // dtilt
	6, // utilt
	3, // first jab
	5 // second jab
};

float attack_damage[] = 
{
	11.0f, // fsmash
	4.0f,  // nair
	6.0f,  // uair
	9.0f, // dair
	10.0f, // fair
	7.0f,  // bair
	6.0f,  // dash tackle
	13.0f, // usmash
	14.0f, // dsmash
	6.0f,  // ftilt
	3.0f,  // dtilt
	4.0f,  // utilt
	2.0f, // first jab
	4.0f // second jab
};

glm::vec2 knockback_vectors[] = 
{
	{ 2.0f, 1.6f },  // fsmash
	{ 1.2f, 1.2f },	 // nair
	{ 0.1f, 2.0f },	 // uair
	{ 0.0f, -1.5f }, // dair
	{ 1.3f, 1.5f },	 // fair
	{ 1.6f, 1.7 },	 // bair
	{ 1.1f, 1.0f },	 // dash tackle
	{ 0.3f, 2.5f },	 // usmash
	{ 0.7f, -2.0 },	 // dsmash
	{ 1.1f, 0.8f },	 // ftilt
	{ 0.2f, 1.7f },	 // dtilt
	{ 0.15f, 2.4f},   // utilt
	{ 0.05f, 0.8f}, // first jab
	{ 0.2f, 1.0f} // second jab
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
	{ 0.32f, 0.04f },	// dtilt
	{ 0.1f, 0.3f},      // utilt
	{ 0.2f, 0.2f },    // first jab
	{ 0.3f, 0.2f }		// second jab
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
	{ 0.35f, 0.1f }, // dtilt
	{ 0.2f, 0.45f }, // utilt
	{ 0.15f, 0.15f },// first jab
	{ 0.15f, 0.15f } // second jab
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
			hitbox_start_frames[i],
			hitbox_stop_frames[i],
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
	if ((int)state <2)
	{
		change_player_animation(player, get_anim((int)state), Loop);
	}

	if (state == ActionState::Falling)
	{
		change_player_animation(player, get_anim(8), Loop);
	}

	if (state == ActionState::Attacking)
	{
		player->Combat.Is_current_attack_resolved = false;
	}

	/*if (state == ActionState::Running)
	{
		printf("previous state: %s\n", get_action_state_name((int)player->ActionState.Action_state));
	}

	if (player->ActionState.Action_state == ActionState::JumpSquat)
	{
		printf("new state: %s\n", get_action_state_name((int)state));
	}*/

	player->ActionState.Previous_action_state = player->ActionState.Action_state;
	player->ActionState.Action_state = state;
	//printf("Player: %i entered state: %i\n", player->ID, state);
}

void Player::set_position_state(State state)
{
	StateC.Previous_position_state = StateC.Current_position_state;
	StateC.Time_in_position_state = 0.0f;

	StateC.Current_position_state.exit(*this);
	StateC.Current_position_state = state;
	StateC.Current_position_state.enter(*this);
}

void Player::set_action_state(State state)
{
	StateC.Previous_action_state = StateC.Current_action_state;
	StateC.Time_in_action_state = 0.0f;

	StateC.Current_action_state.exit(*this);
	StateC.Current_action_state = state;
	StateC.Current_action_state.enter(*this);
}

void Player::set_locomotion_state(State state)
{
	StateC.Previous_locomotion_state = StateC.Current_locomotion_state;
	StateC.Time_in_locomotion_state = 0.0f;

	StateC.Current_locomotion_state.exit(*this);
	StateC.Current_locomotion_state = state;
	StateC.Current_locomotion_state.enter(*this);
}

void Player::perform_grounded_attack(Attack attack)
{
	set_action_state(get_states().Grounded_attack);
}

void Player::perform_aerial_attack(Attack attack)
{
	set_action_state(get_states().Airborne_attack);
}

bool Player::is_facing_travel_direction()
{
	if (Physics.Velocity.x == 0.0f)
	{
		return false;
	} else if (Physics.Velocity.x > 0.0f && !Animation.Is_flipped)
	{
		return true;
	} else if (Physics.Velocity.x < 0.0f && Animation.Is_flipped)
	{
		return true;
	}

	return false;
}

bool Player::is_inputting_in_travel_direction()
{
	if (Input.Left_stick_x == 0.0f)
	{
		return false;
	}
	else if (Physics.Velocity.x > 0.0f && Input.Left_stick_x > 0.0f)
	{
		return true;
	}
	else if (Physics.Velocity.x < 0.0f && Input.Left_stick_x < 0.0f)
	{
		return true;
	}

	return false;
}

bool Player::is_grounded()
{
	RaycastHit hit = RaycastHit();
	return raycast(Transform.Position, glm::vec2(0.0f, -1.0f), 0.1f, hit);
}
