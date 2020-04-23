#include "AnimationSystem.h"

#include "Player.h"

#include "Components/AnimationComponent.h"
#include "Components/TransformComponent.h"

#include "Renderer/Renderer.h"

#include "Animation/Spritesheet.h"
#include "Animation/Sprite.h"

const float pixel_ratio = 0.02f;

void update_animation_system(Player* player, float dt)
{
	AnimationComponent& anim = player->Animation;
	TransformComponent& transform = player->Transform;
	ColliderComponent& collider = player->Collider;

	anim.Current_sprite_time += dt;

	switch (anim.Anim_state)
	{
		case Loop:
			if (anim.Current_sprite_time >= anim.Current_anim->Frame_delay)
			{
				anim.Current_Sprite_Index++;

				if (anim.Current_Sprite_Index == anim.Current_anim->Sprites.size())
				{
					anim.Current_Sprite_Index = 0;
				}

				anim.Current_sprite_time = 0.0f;

				update_player_animation(player, anim.Current_anim->Sprites[anim.Current_Sprite_Index]);
			}
			break;
		case LastFrameStick:
			if (!anim.Has_full_anim_played && anim.Current_sprite_time >= anim.Current_anim->Frame_delay)
			{
				if (anim.Current_Sprite_Index == anim.Current_anim->Sprites.size() - 1)
				{
					anim.Has_full_anim_played = true;
				}
				else
				{
					anim.Current_Sprite_Index++;
				}

				if (anim.Current_Sprite_Index == anim.Current_anim->Sprites.size())
				{
					anim.Current_Sprite_Index = anim.Current_anim->Sprites.size() - 1;
				}

				anim.Current_sprite_time = 0.0f;
			}

			update_player_animation(player, anim.Current_anim->Sprites[anim.Current_Sprite_Index]);
			break;
		case Custom:
			update_player_animation(player, anim.Current_anim->Sprites[anim.Current_Sprite_Index]);
			break;
		default:
			break;
	}

	// HACK call every update because players share VBO and will update each others
	// fix by having separate VBOs or two positions in same and get one draw call but
	// seems difficult with little reward probably just have separate VBOs or keep doing this until it breaks
	update_player_animation(player, anim.Current_anim->Sprites[anim.Current_Sprite_Index]);

	glm::vec2 pos = glm::vec2(transform.Position.x, transform.Position.y);
	glm::vec2 offset = glm::vec2(0.0f, anim.Animation_scale.y / 2);
	
	render_quad(*anim.Current_anim->texture, pos + offset, anim.Animation_scale, anim.Color);

	//queue_GUI_quad_for_rendering(pos, anim.Color, glm::vec3(0.1f));
	glm::vec4 noHitClr = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 hitClr = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//render_quad_outline(pos + offset, transform.Scale, anim.Color);
	collider.Position = pos + offset;
	collider.Scale = transform.Scale;
}

void update_player_animation(Player* player, Sprite* new_sprite)
{
	if (player != nullptr)
	{
		glm::vec2 new_scale = glm::vec2();

		float width_x = new_sprite->Width * pixel_ratio;

		new_scale.x = player->Animation.Is_flipped? - width_x : width_x;
		new_scale.y = new_sprite->Height * pixel_ratio;

		player->Animation.Animation_scale = new_scale;
	}

	update_texture_coordinates(new_sprite);
}
