#include "jppch.h"

#include "PlayerStates.h"

#include "StateInitialization.h"

States states = create_states();

States get_states()
{
	return states;
}
