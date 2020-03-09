#pragma once

#include <glad/glad.h>

struct RenderData
{
	GLuint Quad_VA, Quad_VB, Quad_IB;
	GLuint ShaderID;
};

bool init_renderer();
void quit_renderer();
void render_quad();
