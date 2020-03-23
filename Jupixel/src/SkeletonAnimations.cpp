#include "SkeletonAnimations.h"

#include "Texture2D.h"

#include "Animation/Spritesheet.h"
#include "Animation/Sprite.h"

// movement spritesheets
Spritesheet* idle_spritesheet;
Spritesheet* walk_spritesheet;

// attack spritesheets
Spritesheet* punch_spritesheet;


void load_skeleton_sprites()
{
	Texture2D* texture = nullptr;

	texture = load_texture("assets/textures/Idle_Sheet.png");
	idle_spritesheet = new Spritesheet(texture, 32, 32, 4);

	texture = load_texture("assets/textures/Walk_Sheet.png");
	walk_spritesheet = new Spritesheet(texture, 32, 32, 4);

	texture = load_texture("assets/textures/Punch_Sheet.png");
	punch_spritesheet = new Spritesheet(texture, 64, 32, 9);
}

Spritesheet* get_idle_sheet()
{
	return idle_spritesheet;
}

Spritesheet* get_walk_sheet()
{
	return walk_spritesheet;
}

Spritesheet* get_punch_sheet()
{
	return punch_spritesheet;
}
