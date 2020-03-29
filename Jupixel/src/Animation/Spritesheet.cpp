#include "Spritesheet.h"

#include "Sprite.h"

Spritesheet::Spritesheet(Texture2D* _texture, int _sprite_width, int _sprite_height, int _frames, float frame_delay)
{
	texture = _texture;
	Frame_delay = frame_delay;
	Duration = frame_delay * _frames;

	for (int i = 0; i < _frames; i++)
	{
		Sprite* sprite = new Sprite(texture, _sprite_width, _sprite_height, i);

		Sprites.push_back(sprite);
	}
}
