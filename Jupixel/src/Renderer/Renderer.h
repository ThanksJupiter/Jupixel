#pragma once

#include <glad/glad.h>
#include "glm/glm.hpp"

struct RenderData
{
	GLuint Quad_VA, Quad_VB, Quad_IB;
	GLuint ShaderID;
};

struct RenderObject
{
	glm::vec2 Position = glm::vec2(0.0f);
	bool UseColor = false;
	glm::vec4 Color = glm::vec4(1.0f);
};

bool init_renderer();
void quit_renderer();

void begin_scene();
void clear();
void render();

void queue_quad_for_rendering(glm::vec2& position);
void queue_quad_for_rendering(glm::vec2& position, glm::vec4& color);
void queue_GUI_quad_for_rendering(glm::vec2& position);
void queue_GUI_quad_for_rendering(glm::vec2& position, glm::vec4& color);

void render_quad();
void render_quad(glm::vec2& position);
void render_quad(glm::vec2& position, glm::vec4& color);
void render_quad(glm::vec3& position);
void render_quad(glm::vec3& position, glm::vec4& color);
