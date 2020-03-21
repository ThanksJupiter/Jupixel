#include "Sprite.h"
#include "Texture2D.h"


Sprite::Sprite(Texture2D* texture, int size)
{
	int rows = (int)64 / 32;

	float uv_x = (float)(index % rows) / rows;
	float uv_y = (float)(index / rows) / rows;

	glm::vec2 topRight = glm::vec2(uv_x + 1.0f / rows, uv_y);
	glm::vec2 bottomRight = glm::vec2(uv_x + 1.0f / rows, uv_y + 1.0f / rows);
	glm::vec2 bottomLeft = glm::vec2(uv_x, uv_y + 1.0f / rows);
	glm::vec2 topLeft = glm::vec2(uv_x, uv_y);
}
