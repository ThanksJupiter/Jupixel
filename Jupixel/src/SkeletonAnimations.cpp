#include "SkeletonAnimations.h"

#include "Texture2D.h"

#include "Animation/Spritesheet.h"
#include "Animation/Sprite.h"
#include "Components/ActionStateComponent.h"

#include <stdio.h>
#include <vector>

//#include "json.h"

// movement spritesheets
Spritesheet* idle_spritesheet = nullptr;
Spritesheet* walk_spritesheet = nullptr;

// attack spritesheets
Spritesheet* punch_spritesheet = nullptr;
Spritesheet* nair_spritesheet = nullptr;

std::vector<Spritesheet*> anim_sheets = std::vector<Spritesheet*>();
std::vector<Spritesheet*> attack_anim_sheets = std::vector<Spritesheet*>();

void load_skeleton_sprites()
{
	Texture2D* texture = nullptr;

	// TODO use json to load animations? attacks? god only knows what I'll load with json
	/*FILE* file = nullptr;
	char buffer[1024];
	file = fopen("assets/json/Attacks.json", "r");

	fread(buffer, 1024, 1, file);
	fclose(file);

	struct json_object* parsed_json;
	struct json_object* frames;

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "frames", &frames);

	printf("Frame: %i\n", json_object_get_int(frames));*/

	texture = load_texture("assets/textures/Idle_Sheet.png");
	idle_spritesheet = new Spritesheet(texture, 32, 32, 4, 0.1f);
	anim_sheets.push_back(idle_spritesheet);

	texture = load_texture("assets/textures/Walk_Sheet.png");
	walk_spritesheet = new Spritesheet(texture, 32, 32, 4, 0.1f);
	anim_sheets.push_back(walk_spritesheet);

	texture = load_texture("assets/textures/Punch_Sheet.png");
	punch_spritesheet = new Spritesheet(texture, 64, 32, 9, 0.08f);
	attack_anim_sheets.push_back(punch_spritesheet);

	texture = load_texture("assets/textures/Nair_Sheet.png");
	nair_spritesheet = new Spritesheet(texture, 32, 32, 5, 0.1f);
	attack_anim_sheets.push_back(nair_spritesheet);

	texture = load_texture("assets/textures/Uair_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 6, 0.07f));

	texture = load_texture("assets/textures/Dair_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 8, 0.08f));

	texture = load_texture("assets/textures/Fair_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 7, 0.08f));

	texture = load_texture("assets/textures/Bair_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 11, 0.05f));
}

Spritesheet* get_anim(int index)
{
	return anim_sheets[index];
}

Spritesheet* get_attack_anim(int index)
{
	return attack_anim_sheets[index];
}

int get_attack_anim_count()
{
	return attack_anim_sheets.size();
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
