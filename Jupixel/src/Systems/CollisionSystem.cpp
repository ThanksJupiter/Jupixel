#include "CollisionSystem.h"

#include "Player.h"
#include "World.h"
#include "glm/glm.hpp"

#include "Physics/CollisionTestRequest.h"
#include "Components/ColliderComponent.h"

#include "Renderer/Renderer.h"
#include <vector>

std::vector<CollisionTestRequest> requests = std::vector<CollisionTestRequest>();
glm::vec2 point_scale = glm::vec2(0.05f, 0.05f);

void queue_collision(CollisionTestRequest request)
{
	requests.push_back(request);
}

void test_collisions()
{
	for (int i = 0; i < requests.size(); i++)
	{
		CollisionTestRequest& request = requests[i];

		if (request.Is_resolved)
		{
			continue;
		}

		Player* otherP = request.Target;
		ColliderComponent* c = &request.Collider;
		ColliderComponent* otherC = &request.Target->Collider;

		c->Position = request.Collider.Position;
		glm::vec2& p = c->Position;

		glm::vec2 hitbox_pos = p;

		hitbox_pos.x += c->Offset.x;
		hitbox_pos.y += c->Offset.y;

		glm::vec2 scale = c->Scale;
		
		render_quad(hitbox_pos, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), point_scale);
		render_quad_outline(hitbox_pos, scale, glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));

		glm::vec2 a_max = glm::vec2(hitbox_pos.x + scale.x * 0.5f, hitbox_pos.y + scale.y * 0.5f);
		glm::vec2 a_min = glm::vec2(hitbox_pos.x - scale.x * 0.5f, hitbox_pos.y - scale.y * 0.5f);

		glm::vec2 b_pos = otherC->Position;
		glm::vec2 b_scale = otherC->Scale;

		glm::vec2 b_max = glm::vec2(b_pos.x + b_scale.x * 0.5f, b_pos.y + b_scale.y * 0.5f);
		glm::vec2 b_min = glm::vec2(b_pos.x - b_scale.x * 0.5f, b_pos.y - b_scale.y * 0.5f);

		bool no_overlap_y = a_max.y < b_min.y || a_min.y > b_max.y;
		bool no_overlap_x = a_max.x < b_min.x || a_min.x > b_max.x;

		if (no_overlap_x || no_overlap_y)
		{
			requests.clear();
			return;
		}
		else
		{
			set_time_scale(0.0001f);
			request.Target->Reset_time_scale_on_land = true;
			request.Is_resolved = true;
			otherC->Is_colliding = true;
			otherC->Is_hit = true;
			request.Instigator->Combat.Is_current_attack_resolved = true;
			otherC->Pending_knockback.y = request.Instigator->Combat.Current_attack->Knockback_direction.y;
			otherC->Pending_damage = request.Instigator->Combat.Current_attack->Damage;

			if (request.Instigator->Combat.Current_attack == &request.Instigator->Combat.Attacks[5])
			{
				otherC->Pending_knockback.x = request.Instigator->Animation.Is_flipped ?
					request.Instigator->Combat.Current_attack->Knockback_direction.x :
					-request.Instigator->Combat.Current_attack->Knockback_direction.x;

				otherC->Flip = request.Instigator->Animation.Is_flipped;
			}
			else
			{
				otherC->Pending_knockback.x = request.Instigator->Animation.Is_flipped ?
					-request.Instigator->Combat.Current_attack->Knockback_direction.x :
					request.Instigator->Combat.Current_attack->Knockback_direction.x;

				otherC->Flip = !request.Instigator->Animation.Is_flipped;
			}

		}
	}

	requests.clear();
}
