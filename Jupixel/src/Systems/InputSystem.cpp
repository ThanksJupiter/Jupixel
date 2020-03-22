#include "InputSystem.h"

#include "UserInput.h"
#include "ECS/Components/InputComponent.h"
#include "ECS/ComponentLists.h"
#include "Key.h"

void update_input_system(InputComponent* i, ComponentLists* components)
{
	// TODO sort out input somehow, somewhy
	if (i->entity_id == 0)
	{
		i->x = is_key_pressed(KeyCode::D) ? 1 :
			is_key_pressed(KeyCode::A) ? -1 :
			0;

		i->y = is_key_pressed(KeyCode::W) ? 1 :
			is_key_pressed(KeyCode::S) ? -1 :
			0;

		i->jump = is_key_pressed(KeyCode::Q);
		i->attack = is_key_pressed(KeyCode::E);
	}
	else
	{
		i->x = is_key_pressed(KeyCode::L) ? 1 :
			is_key_pressed(KeyCode::J) ? -1 :
			0;

		i->y = is_key_pressed(KeyCode::I) ? 1 :
			is_key_pressed(KeyCode::K) ? -1 :
			0;

		i->jump = is_key_pressed(KeyCode::U);
		i->attack = is_key_pressed(KeyCode::O);
	}
}
