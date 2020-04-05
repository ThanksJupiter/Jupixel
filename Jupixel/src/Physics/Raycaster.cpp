#include "Raycaster.h"

#include "Renderer/Renderer.h"

std::vector<ColliderComponent*> current_colliders = std::vector<ColliderComponent*>();

// Only returns normal opposite of direction
bool raycast(glm::vec2 from_position, glm::vec2 direction, float distance, RaycastHit& hit)
{
	for (int i = 0; i < current_colliders.size(); i++)
	{
		ColliderComponent& col = *current_colliders[0];

		glm::vec2 point = from_position + (direction * distance);

		glm::vec2 b_pos = col.Position;
		glm::vec2 b_scale = col.Scale;

		glm::vec2 b_max = glm::vec2(b_pos.x + b_scale.x * 0.5f, b_pos.y + b_scale.y * 0.5f);
		glm::vec2 b_min = glm::vec2(b_pos.x - b_scale.x * 0.5f, b_pos.y - b_scale.y * 0.5f);

		//render_quad(point, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.05f, 0.05f));
		//render_quad(col.Position, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), col.Scale);

		glm::vec4 hitClr = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		glm::vec4 noHitClr = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		if (point.x < b_max.x && point.x > b_min.x &&
			point.y < b_max.y && point.y > b_min.y)
		{
			float col_pos_y = col.Position.y + (col.Scale.y * 0.5f);
			float col_pos_x = col.Position.x + (col.Scale.x * 0.5f);

			glm::vec2 hit_point = glm::vec2(
				col_pos_x, // TODO convert to extents to avoid * 0.5f
				col_pos_y
				);
			hit.point = hit_point;
			hit.normal = -direction; // TODO actually calculate normal

			//render_quad(point, hitClr, glm::vec2(0.05f, 0.05f));
			//render_quad(col.Position, hitClr, col.Scale);

			return true;
		}
		else
		{
			//render_quad(point, noHitClr, glm::vec2(0.05f, 0.05f));
			//render_quad(col.Position, noHitClr, col.Scale);
		}
	}

	return false;
}

void set_target_colliders(std::vector<ColliderComponent*> colliders)
{
	current_colliders = colliders;
}

void add_target_collider(ColliderComponent* collider)
{
	current_colliders.push_back(collider);
}
