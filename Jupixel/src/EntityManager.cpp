#include "jppch.h"

#include "EntityManager.h"
#include "Components/InputComponent.h"

using EntityID = unsigned int;

std::vector<EntityID> ENTITY_IDS = std::vector<EntityID>();

std::map<EntityID, InputComponent> entity_inputs;

void update_entities()
{
	
}
