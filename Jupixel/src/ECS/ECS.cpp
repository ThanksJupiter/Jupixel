#include "ECS.h"
#include "ComponentLists.h"
#include "UserInput.h"
#include "Key.h"
#include "Renderer/Renderer.h"

int ENTITIES = 0;
float dt = 0;

void ecs_update(ComponentLists* components, float deltatime)
{
	dt = deltatime; 

	for (int i = 0; i < components->total_input_components; i++)
	{
		update_input_system(&components->input_components[i], components);
	}

	for (int i = 0; i < components->total_position_components; i++)
	{
		update_position_system(&components->position_components[i], components);
	}

	for (int i = 0; i < components->total_render_components; i++)
	{
		update_render_system(&components->render_components[i], components);
	}
}

void create_entity(ComponentLists* components)
{
	int id = ENTITIES++;
	components->position_components[id].entity_id = id;
	components->total_position_components++;

	components->velocity_components[id].entity_id = id;
	components->total_velocity_components++;

	components->input_components[id].entity_id = id;
	components->total_input_components++;

	components->render_components[id].entity_id = id;
	components->total_render_components++;
}

void create_entity(ComponentLists* components, glm::vec4 color)
{
	create_entity(components);
	components->render_components[components->total_render_components - 1].Color = color;
}

void create_non_input_entity(ComponentLists* components)
{
	int id = ENTITIES++;
	components->position_components[id].entity_id = id;
	components->total_position_components++;

	components->render_components[id].entity_id = id;
	components->total_render_components++;
}

void update_position_system(Position* p, ComponentLists* components)
{
	Input* i = &components->input_components[p->entity_id];
	Velocity* v = &components->velocity_components[p->entity_id];

	v->x = i->x * 4.0f;

	p->x += v->x * dt;

	if (p->y > -1.0f)
	{
		v->y -= 10 * dt;
		p->y += v->y * dt;
	}
	else
	{
		v->y = 0;
		p->y = -1.0f;

		if (i->jump)
		{
			v->y += 5;
			p->y += 0.01;
		}
	}
}

void update_input_system(Input* i, ComponentLists* components)
{
	if (i->entity_id == 0)
	{
		i->x = is_key_pressed(KeyCode::D) ?  1 :
			   is_key_pressed(KeyCode::A) ? -1 :
		   									 0;

		i->y = is_key_pressed(KeyCode::W) ?  1 :
			   is_key_pressed(KeyCode::S) ? -1 :
											 0;

		i->jump = is_key_pressed(KeyCode::Space);
	}
	else
	{
		i->x = is_key_pressed(KeyCode::L) ?  1 :
			   is_key_pressed(KeyCode::J) ? -1 :
											 0;

		i->y = is_key_pressed(KeyCode::I) ?  1 :
			   is_key_pressed(KeyCode::K) ? -1 :
											 0;

		i->jump = is_key_pressed(KeyCode::U);
	}
}

void update_render_system(Render* r, ComponentLists* components)
{
	Position* p = &components->position_components[r->entity_id];

	glm::vec2 pos = glm::vec2(p->x, p->y);
	queue_quad_for_rendering(pos, r->Color);

	glm::vec2 offset = glm::vec2(0.0f, 0.5f);
	queue_GUI_quad_for_rendering(pos + offset, glm::vec4(0.0f, 0.0f, 1.0f, 0.5f));
}
