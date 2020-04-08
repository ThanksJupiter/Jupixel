#include "VFXSystem.h"

#include <vector>

#include "Renderer/Renderer.h"

#include "Components/VFXComponent.h"
#include "Animation/Spritesheet.h"
#include "Components/TransformComponent.h"
#include "Animation/Sprite.h"
#include "Texture2D.h"

std::vector<VFXComponent> effects = std::vector<VFXComponent>();

void vfx_spawn_effect(Spritesheet* sheet, glm::vec2 position, glm::vec4 color, TransformComponent* transform /*= nullptr*/, PhysicsComponent* physics /*= nullptr*/, AnimationState state /*= AnimationState::Loop*/, int repeat_cycles /*= 0*/)
{
	for (int i = 0; i < effects.size(); i++)
	{
		if (!effects[i].Is_active)
		{
			VFXComponent& vfx = effects[i];

			vfx.Data.TextureID = sheet->texture->ID;
			vfx.Position = position;
			vfx.Transform = transform;
			vfx.physics = physics;
			vfx.Repeat_count = repeat_cycles;
			vfx.Current_repeats = 0;
			vfx.Is_active = true;
			vfx.Anim.Current_anim = sheet;
			vfx.Anim.Anim_state = state;
			vfx.Anim.Current_Sprite_Index = 0;
			vfx.Anim.Has_full_anim_played = false;
			vfx.Anim.Color = color;

			return;
		}
	}

	VFXComponent vfx = VFXComponent();

	vfx.Data = generate_textured_quad_buffers();

	vfx.Data.TextureID = sheet->texture->ID;
	vfx.Position = position;
	vfx.Transform = transform;
	vfx.physics = physics;
	vfx.Repeat_count = repeat_cycles;
	vfx.Current_repeats = 0;
	vfx.Is_active = true;
	vfx.Anim.Current_anim = sheet;
	vfx.Anim.Anim_state = state;
	vfx.Anim.Color = color;

	effects.push_back(vfx);
}

void vfx_update(float dt)
{
	for (int i = 0; i < effects.size(); i++)
	{
		if (effects[i].Is_active)
		{
			vfx_update_effect(effects[i], dt);
		}
	}
}

void vfx_update_effect(VFXComponent& effect, float dt)
{
	AnimationComponent& anim = effect.Anim;

	anim.Current_sprite_time += dt;

	if (anim.Anim_state == Loop)
	{
		if (anim.Current_sprite_time >= anim.Current_anim->Frame_delay)
		{
			anim.Current_Sprite_Index++;

			if (anim.Current_Sprite_Index == anim.Current_anim->Sprites.size())
			{
				anim.Current_Sprite_Index = 0;
				effect.Current_repeats ++;

				if (effect.Current_repeats == effect.Repeat_count)
				{
					effect.Is_active = false;
				}
			}

			anim.Current_sprite_time = 0.0f;

			vfx_update_effect_data(effect, anim.Current_anim->Sprites[anim.Current_Sprite_Index]);
		}
	}
	else if (anim.Anim_state == LastFrameStick)
	{
		if (!anim.Has_full_anim_played && anim.Current_sprite_time >= anim.Current_anim->Frame_delay)
		{
			if (anim.Current_Sprite_Index == anim.Current_anim->Sprites.size() - 1)
			{
				anim.Has_full_anim_played = true;
				effect.Is_active = false;
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

		vfx_update_effect_data(effect, anim.Current_anim->Sprites[anim.Current_Sprite_Index]);
	}

	if (effect.Transform)
	{
		effect.Position = effect.Transform->Position;
	}

	render_quad(*anim.Current_anim->texture, effect.Data.VAO, effect.Position, anim.Animation_scale, anim.Color);

	//queue_GUI_quad_for_rendering(effect.Position, anim.Color, glm::vec3(0.1f));
	glm::vec4 noHitClr = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 hitClr = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//render_quad_outline(pos, transform.Scale, anim.Color);
}

void vfx_update_effect_data(VFXComponent& effect, Sprite* new_sprite)
{
	glm::vec2 new_scale = glm::vec2();

	float width_x = new_sprite->Width * 0.02f;

	new_scale.x = effect.Anim.Is_flipped ? -width_x : width_x;
	new_scale.y = new_sprite->Height * 0.02f;

	effect.Anim.Animation_scale = new_scale;

	update_texture_coordinates(new_sprite, effect.Data.VBO);
}
