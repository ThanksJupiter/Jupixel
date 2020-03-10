#pragma once

#include "glm/fwd.hpp"

struct Texture2DData
{
	uint32_t ID = 0;
	char* path = "";

	uint32_t width = 0;
	uint32_t height = 0;
};

void load_texture(const std::string& path);
void bind_texture(uint32_t slot = 0);
