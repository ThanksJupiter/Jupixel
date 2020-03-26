#include "CollisionSystem.h"

#include "Player.h"
#include "glm/glm.hpp"

#include "Physics/CollisionTestRequest.h"
#include "Components/ColliderComponent.h"

#include "Renderer/Renderer.h"
#include <vector>

std::vector<CollisionTestRequest*> requests = std::vector<CollisionTestRequest*>();

void queue_collision(CollisionTestRequest* request)
{
	requests.push_back(request);
}

void test_collisions()
{
	for (int i = 0; i < requests.size(); i++)
	{
		CollisionTestRequest& request = *requests[i];

		if (request.Is_resolved)
		{
			continue;
		}

		Player* otherP = request.Target;
		AnimationComponent anim = request.Instigator->Animation;
		CollisionTestRequest* c = &request;
		ColliderComponent* otherC = &request.Target->Collider;

		glm::vec2& p = request.Collider.Position;

		// TODO don't calculate this here obviously my man you know better than that the little ofc
		glm::vec2 hitbox_pos = p;
		float value_to_add = 0.35f * (request.Instigator->Animation.Is_flipped ? -1 : 1);
		hitbox_pos.x += value_to_add;
		hitbox_pos.y += 0.2f;

		glm::vec2 scale = glm::vec2(0.4f, 0.2f);
		glm::vec2 point_scale = glm::vec2(0.05f, 0.05f);

		render_quad(hitbox_pos, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), point_scale);
		render_quad_outline(hitbox_pos, scale, glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));

		glm::vec2 a_max = glm::vec2(hitbox_pos.x + scale.x / 2, hitbox_pos.y + scale.y / 2);
		glm::vec2 a_min = glm::vec2(hitbox_pos.x - scale.x / 2, hitbox_pos.y - scale.y / 2);

		glm::vec2 b_pos = otherC->Position;
		glm::vec2 b_scale = otherC->Scale;

		glm::vec2 b_max = glm::vec2(b_pos.x + b_scale.x / 2, b_pos.y + b_scale.y / 2);
		glm::vec2 b_min = glm::vec2(b_pos.x - b_scale.x / 2, b_pos.y - b_scale.y / 2);

		bool no_overlap_y = a_max.y < b_min.y || a_min.y > b_max.y;
		bool no_overlap_x = a_max.x < b_min.x || a_min.x > b_max.x;

		if (no_overlap_x || no_overlap_y)
		{
			requests.clear();
			return;
		}
		else
		{
			request.Is_resolved = true;
			otherC->Is_colliding = true;
			otherC->Is_hit = true;
		}
	}

	requests.clear();
}
