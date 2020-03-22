#include "Sprite.h"

#include "Texture2D.h"

Sprite::Sprite(Texture2D* _texture, int _width, int _height, int _index)
{
	texture = _texture;
	index = _index;
	width = _width;
	height = _height;

	int rows = (int)texture->width / width;

	float uv_x = (float)(index % rows) / rows;
	float uv_y = (float)(index / rows) / rows;

	topRight =    glm::vec2(uv_x + 1.0f / rows, uv_y);
	bottomRight = glm::vec2(uv_x + 1.0f / rows, uv_y + 1.0f / rows);
	bottomLeft =  glm::vec2(uv_x,				uv_y + 1.0f / rows);
	topLeft =     glm::vec2(uv_x,				uv_y);
}
