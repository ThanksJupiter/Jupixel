#pragma once

#include "glm/glm.hpp"

struct ComponentLists;
struct PositionComponent;
struct InputComponent;
struct RenderComponent;
struct ColliderComponent;

void ecs_update(ComponentLists* components, float deltatime);
void create_entity(ComponentLists* components);
void create_entity(ComponentLists* components, glm::vec4 color);
void create_non_input_entity(ComponentLists* components);
void update_position_system(PositionComponent* p, ComponentLists* components);
void update_render_system(RenderComponent* r, ComponentLists* components);
void update_collision_system(ColliderComponent* c, ComponentLists* components);
