#pragma once

#include <glad/glad.h>
#include "glm/glm.hpp"

struct RenderData
{
	GLuint Quad_VA, Quad_VB, Quad_IB;
	GLuint ShaderID;
};

bool init_renderer();
void quit_renderer();

void render_quad();
void render_quad(glm::vec2& position);
