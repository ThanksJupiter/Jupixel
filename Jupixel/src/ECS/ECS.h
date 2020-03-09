#pragma once

struct ComponentLists;
struct Position;
struct Input;
struct Render;

void ecs_update(ComponentLists* components, float deltatime);
void create_entity(ComponentLists* components);
void create_non_input_entity(ComponentLists* components);
void update_position_system(Position* p, ComponentLists* components);
void update_input_system(Input* i, ComponentLists* components);
void update_render_system(Render* r, ComponentLists* components);
