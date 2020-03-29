#pragma once
#include <vector>

struct Texture2D;
struct Sprite;

struct Spritesheet
{
	Spritesheet(Texture2D* _texture, int _sprite_width, int _sprite_height, int _frames, float frame_delay);

	Texture2D* texture = nullptr;
	std::vector<Sprite*> Sprites = std::vector<Sprite*>();
	float Frame_delay = 0.0f;
	float Duration = 0.0f;
};
