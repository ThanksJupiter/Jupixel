#include "AnimationSystem.h"

#include "Player.h"

#include "Components/AnimationComponent.h"
#include "Components/TransformComponent.h"

#include "Renderer/Renderer.h"

#include "Animation/Spritesheet.h"
#include "Animation/Sprite.h"

void update_animation_system(Player* _player, float _dt)
{
	AnimationComponent& anim = _player->animation;
	TransformComponent& transform = _player->transform;

	anim.currentSpriteTime += _dt * 0.5;

	if (anim.currentSpriteTime >= anim.nextSpriteDelay)
	{
		anim.currentSpriteIndex++;
		if (anim.currentSpriteIndex == anim.current_anim->sprites.size())
		{
			anim.currentSpriteIndex = 0;
		}

		anim.currentSpriteTime = 0.0f;
		update_texture_coordinates(anim.current_anim->sprites[anim.currentSpriteIndex]);
	}

	glm::vec2 pos = glm::vec2(transform.Position.x, transform.Position.y);
	//queue_quad_for_rendering(pos, r->Color);
	glm::vec2 offset = glm::vec2(0.0f, 0.5f);
	
	render_quad(*anim.current_anim->texture, transform.Position + offset, transform.Scale, anim.Color);


	queue_GUI_quad_for_rendering(pos, glm::vec4(anim.Color.r, anim.Color.g, anim.Color.b, 1.0f), glm::vec3(0.1f));

	glm::vec4 noHitClr = glm::vec4(0.0f, 1.0f, 0.0f, 0.3f);
	glm::vec4 hitClr = glm::vec4(1.0f, 0.0f, 0.0f, 0.3f);
	render_quad_outline(pos + offset, glm::vec2(transform.Scale), noHitClr);
}
