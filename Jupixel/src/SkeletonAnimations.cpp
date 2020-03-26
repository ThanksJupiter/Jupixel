#include "SkeletonAnimations.h"

#include "Texture2D.h"

#include "Animation/Spritesheet.h"
#include "Animation/Sprite.h"

#include <stdio.h>

#include "json.h"

// movement spritesheets
Spritesheet* idle_spritesheet;
Spritesheet* walk_spritesheet;

// attack spritesheets
Spritesheet* punch_spritesheet;


void load_skeleton_sprites()
{
	Texture2D* texture = nullptr;

	// TODO use json to load animations? attacks? god only knows what I'll load with json
	FILE* file = nullptr;
	char buffer[1024];
	file = fopen("assets/json/Attacks.json", "r");

	fread(buffer, 1024, 1, file);
	fclose(file);

	struct json_object* parsed_json;
	struct json_object* frames;

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "frames", &frames);

	printf("Frame: %i\n", json_object_get_int(frames));

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
