#include "Sprite.h"

#include "Texture2D.h"

Sprite::Sprite(Texture2D* texture, int width, int height, int index)
{
	Texture = texture;
	Index = index;
	Width = width;
	Height = height;

	int rows = (int)Texture->width / Width;
	int columns = (int)Texture->height / Height;

	float uv_x = (float)(Index % rows) / rows;
	float uv_y = (float)(Index / rows) / columns;

	TopRight =    glm::vec2(uv_x + 1.0f / rows, uv_y);
	BottomRight = glm::vec2(uv_x + 1.0f / rows, uv_y + 1.0f / columns);
	BottomLeft =  glm::vec2(uv_x,				uv_y + 1.0f / columns);
	TopLeft =     glm::vec2(uv_x,				uv_y);
}
