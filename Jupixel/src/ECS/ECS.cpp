#include "ECS.h"
#include "ComponentLists.h"
#include "UserInput.h"
#include "Key.h"
#include "Renderer/Renderer.h"
#include "Texture2D.h"
#include "Shader.h"

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

	for (int i = 0; i < components->total_collision_components; i++)
	{
		update_collision_system(&components->collision_components[i], components);
	}

	for (int i = 0; i < components->total_render_components; i++)
	{
		update_render_system(&components->render_components[i], components);
	}
}

void create_entity(ComponentLists* components)
{
	int id = ENTITIES++;

	// Position
	components->position_components[id].entity_id = id;
	components->total_position_components++;

	// Velocity
	components->velocity_components[id].entity_id = id;
	components->total_velocity_components++;

	// Input
	components->input_components[id].entity_id = id;
	components->total_input_components++;

	// Render
	components->render_components[id].entity_id = id;
	RenderComponent* rendComp = &components->render_components[id];
	rendComp->texture = load_texture("assets/textures/skel.png");
	components->total_render_components++;

	// Collision
	components->collision_components[id].entity_id = id;
	components->total_collision_components++;
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

void update_position_system(PositionComponent* p, ComponentLists* components)
{
	InputComponent* i = &components->input_components[p->entity_id];
	VelocityComponent* v = &components->velocity_components[p->entity_id];
	ColliderComponent* c = &components->collision_components[p->entity_id];

	v->x = i->x * 4.0f;

	p->x += v->x * dt;

	if (p->y > -1.0f)
	{
		v->y -= 10 * dt;
		p->y += v->y * dt;

		// HACK cannot be hit in air to prevent is_hit from triggering another jump when landing
		// because of getting hit while jumping
		c->is_hit = false;
	}
	else
	{
		v->y = 0;
		p->y = -1.0f;

		if (i->jump || c->is_hit)
		{
			v->y += 5;
			p->y += 0.01;
			c->is_hit = false;
		}
	}
}

void update_input_system(InputComponent* i, ComponentLists* components)
{
	// TODO sort out input somehow, somewhy
	if (i->entity_id == 0)
	{
		i->x = is_key_pressed(KeyCode::D) ?  1 :
			   is_key_pressed(KeyCode::A) ? -1 :
		   									 0;

		i->y = is_key_pressed(KeyCode::W) ?  1 :
			   is_key_pressed(KeyCode::S) ? -1 :
											 0;

		i->jump =   is_key_pressed(KeyCode::Q);
		i->attack = is_key_pressed(KeyCode::E);
	}
	else
	{
		i->x = is_key_pressed(KeyCode::L) ?  1 :
			   is_key_pressed(KeyCode::J) ? -1 :
											 0;

		i->y = is_key_pressed(KeyCode::I) ?  1 :
			   is_key_pressed(KeyCode::K) ? -1 :
											 0;

		i->jump =   is_key_pressed(KeyCode::U);
		i->attack = is_key_pressed(KeyCode::O);
	}
}

void update_render_system(RenderComponent* r, ComponentLists* components)
{
	PositionComponent* p = &components->position_components[r->entity_id];
	InputComponent* i = &components->input_components[p->entity_id];
	ColliderComponent* c = &components->collision_components[r->entity_id];
	r->x = p->x;
	r->y = p->y;

	glm::vec2 pos = glm::vec2(p->x, p->y);
	//queue_quad_for_rendering(pos, r->Color);
	queue_quad_for_rendering(r);

	if (c->is_active)
	{
		glm::vec4 noHitClr = glm::vec4(0.0f, 1.0f, 0.0f, 0.3f);
		glm::vec4 hitClr = glm::vec4(1.0f, 0.0f, 0.0f, 0.3f);

		queue_GUI_quad_for_rendering(pos, !c->is_colliding ? noHitClr : hitClr, glm::vec3(c->scale));
	}
}

void update_collision_system(ColliderComponent* c, ComponentLists* components)
{
	PositionComponent* p = &components->position_components[c->entity_id];
	InputComponent* i = &components->input_components[p->entity_id];

	c->is_active = i->attack;
	c->is_colliding = false;

	for (int i = 0; i < components->total_collision_components; i++)
	{
		if (i == c->entity_id || !c->is_active)
		{
			continue;
		}
		
		ColliderComponent* otherC = &components->collision_components[i];
		PositionComponent* otherP = &components->position_components[i];

		if (p->x < otherP->x + otherC->scale &&
			p->x + c->scale > otherP->x &&
			p->y < otherP->y + otherC->scale &&
			p->y + c->scale > otherP->y)
		{
			c->is_colliding = true;
			otherC->is_colliding = true;
			otherC->is_hit = true;
		}
	}
}
