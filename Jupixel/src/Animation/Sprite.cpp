#include "Sprite.h"

#include "Texture2D.h"

Sprite::Sprite(Texture2D* _texture, int _size, int _index)
{
	texture = _texture;
	index = _index;
	size = _size;

	int rows = (int)texture->width / size;

	float uv_x = (float)(index % rows) / rows;
	float uv_y = (float)(index / rows) / rows;

	topRight = glm::vec2(uv_x + 1.0f / rows, uv_y);
	bottomRight = glm::vec2(uv_x + 1.0f / rows, uv_y + 1.0f / rows);
	bottomLeft = glm::vec2(uv_x, uv_y + 1.0f / rows);
	topLeft = glm::vec2(uv_x, uv_y);
}
