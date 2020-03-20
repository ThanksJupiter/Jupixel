#pragma once

#include "glm/fwd.hpp"
#include <string>

struct Texture2D
{
	uint32_t ID = 0;
	std::string path = "";

	uint32_t width = 0;
	uint32_t height = 0;
};

unsigned int load_texture(const std::string& path);
void bind_texture();
Texture2D get_texture();
