#pragma once

#include "glm/fwd.hpp"
#include <string>

struct Texture2D
{
	Texture2D() {}

	Texture2D(uint32_t _id, std::string _path, uint32_t _width, uint32_t _height)
		: ID(_id), path(_path), width(_width), height(_height) { }

	uint32_t ID = 0;
	std::string path = "";

	uint32_t width = 0;
	uint32_t height = 0;
};

Texture2D* load_texture(const std::string& path);
void bind_texture(unsigned int textureID);
