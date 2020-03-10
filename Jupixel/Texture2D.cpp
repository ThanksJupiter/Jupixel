#include "Texture2D.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <string>

Texture2DData texture;

int TEXTURE_AMOUNT = 0;

void load_texture(const std::string& path)
{
	Texture2DData& d = texture;

	d.path = path.c_str;

	int width, height, channels;

	stbi_set_flip_vertically_on_load(1);

	stbi_uc* data = nullptr;
	data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (data == nullptr)
	{
		printf("Failed to load image");
	}

	d.width = width;
	d.height = height;

	glCreateTextures(GL_TEXTURE_2D, 1, &d.ID);
	glTextureStorage2D(d.ID, 1, GL_RGBA8, width, height);

	glTextureParameteri(d.ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(d.ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTextureParameteri(d.ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTextureParameteri(d.ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(d.ID, 0, 0, 0, width, height, GL_RGB8, GL_UNSIGNED_BYTE, data);

	TEXTURE_AMOUNT++;

	stbi_image_free(data);
}

void bind_texture(uint32_t slot /*= 0*/)
{
	glBindTextureUnit(slot, texture.ID);
}
