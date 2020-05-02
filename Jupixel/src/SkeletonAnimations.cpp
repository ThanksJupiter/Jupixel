#include "SkeletonAnimations.h"

#include "Texture2D.h"

#include "Animation/Spritesheet.h"
#include "Animation/Sprite.h"
#include "Components/ActionStateComponent.h"

#include <stdio.h>
#include <vector>

//#include "json.h"

std::vector<Spritesheet*> anim_sheets = std::vector<Spritesheet*>();
std::vector<Spritesheet*> attack_anim_sheets = std::vector<Spritesheet*>();
std::vector<Spritesheet*> vfx_sheets = std::vector<Spritesheet*>();

void load_skeleton_sprites()
{
	// TODO use json to load animations? attacks? god only knows what I'll load with json
	// TODO live reload anim data for fine-tuning hitboxes/knockback etc
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

	Texture2D* texture = nullptr;

	texture = load_texture("assets/textures/Idle_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 4, 0.1f));

	texture = load_texture("assets/textures/Walk_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 4, 0.1f));

	texture = load_texture("assets/textures/Crouch_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 3, 0.1f));

	texture = load_texture("assets/textures/Knockback_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 7, 0.1f));

	texture = load_texture("assets/textures/Run_Sheet.png"); // 5
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 6, 0.1f));

	texture = load_texture("assets/textures/Turn_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 4, 0.1f));

	texture = load_texture("assets/textures/Knockdown_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 5, 0.1f));

	texture = load_texture("assets/textures/Ledgegrab_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 6, 0.06f));

	texture = load_texture("assets/textures/Falling_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 4, 0.1f));

	texture = load_texture("assets/textures/Get_Up_Sheet.png"); // 10
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 3, 0.15f));

	texture = load_texture("assets/textures/Ledge_Balance_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 8, 0.1f));

	texture = load_texture("assets/textures/High_Block_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 3, 0.1f));

	texture = load_texture("assets/textures/Low_Block_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 4, 0.1f));

	texture = load_texture("assets/textures/Reach_Sheet.png");
	anim_sheets.push_back(new Spritesheet(texture, 32, 32, 5, 0.1f));

	//  -------------------------------------------------------------------------------------
	//  ------------------- SMASH ATTACKS ---------------------------------------------------
	//  -------------------------------------------------------------------------------------

	texture = load_texture("assets/textures/Fsmash_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 64, 32, 9, 0.08f));

	texture = load_texture("assets/textures/Nair_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 7, 0.1f));

	texture = load_texture("assets/textures/Uair_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 6, 0.07f));

	texture = load_texture("assets/textures/Dair_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 8, 0.08f));

	texture = load_texture("assets/textures/Fair_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 8, 0.08f)); // 5

	texture = load_texture("assets/textures/Bair_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 9, 0.05f));

	texture = load_texture("assets/textures/Dash_Tackle_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 8, 0.08f));

	texture = load_texture("assets/textures/Usmash_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 64, 32, 10, 0.08f));

	texture = load_texture("assets/textures/Dsmash_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 15, 0.08f));

	texture = load_texture("assets/textures/Ftilt_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 5, 0.1f)); // 10

	texture = load_texture("assets/textures/Dtilt_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 64, 32, 7, 0.05f));

	texture = load_texture("assets/textures/Utilt_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 32, 32, 7, 0.05f));

	texture = load_texture("assets/textures/Jab_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 64, 32, 8, 0.1f));

	// HACK ultra hack, load jab anim twice to have it count for creating two new attacks :(
	texture = load_texture("assets/textures/Jab_Sheet.png");
	attack_anim_sheets.push_back(new Spritesheet(texture, 64, 32, 8, 0.1f));

	//  -------------------------------------------------------------------------------------
	//  ------------------- VFX -------------------------------------------------------------
	//  -------------------------------------------------------------------------------------

	texture = load_texture("assets/textures/Ledgegrab_VFX_Sheet.png");
	vfx_sheets.push_back(new Spritesheet(texture, 8, 8, 7, 0.02));

	texture = load_texture("assets/textures/Hit_Effect_01_Sheet.png");
	vfx_sheets.push_back(new Spritesheet(texture, 32, 32, 6, 0.06));

	texture = load_texture("assets/textures/Knockback_Dust_Sheet.png");
	vfx_sheets.push_back(new Spritesheet(texture, 16, 16, 9, 0.05));

	texture = load_texture("assets/textures/Death_effect_Sheet.png");
	vfx_sheets.push_back(new Spritesheet(texture, 128, 128, 7, 0.01)); // 4
}

Spritesheet* get_anim(int index)
{
	return anim_sheets[index];
}

Spritesheet* get_attack_anim(int index)
{
	return attack_anim_sheets[index];
}

Spritesheet* get_vfx_anim(int index)
{
	return vfx_sheets[index];
}

int get_attack_anim_count()
{
	return attack_anim_sheets.size();
}
