#include "Texture2D.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <string>

Texture2D* load_texture(const std::string& path)
{
	uint32_t texID;

	int width, height, channels;

	//stbi_set_flip_vertically_on_load(1);

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

	glTextureParameteri(texID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(texID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	return new Texture2D(texID, path, width, height);
}

void bind_texture(unsigned int textureID)
{
	glBindTextureUnit(0, textureID);
}
