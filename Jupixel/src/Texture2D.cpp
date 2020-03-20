#include "Texture2D.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <string>

Texture2D texture;

int TEXTURE_AMOUNT = 0;

unsigned int load_texture(const std::string& path)
{
	unsigned int texID;

	//d.path = path;

	int width, height, channels;

	stbi_set_flip_vertically_on_load(1);

	stbi_uc* data = nullptr;
	data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (data == nullptr)
	{
		printf("Failed to load image");
	}
	
	glCreateTextures(GL_TEXTURE_2D, 1, &texID);
	glTextureStorage2D(texID, 1, GL_RGBA8, width, height);

	glTextureParameteri(texID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(texID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTextureSubImage2D(texID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	return texID;
}

void bind_texture()
{
	glBindTextureUnit(0, texture.ID);
}

Texture2D get_texture()
{
	return texture;
}
