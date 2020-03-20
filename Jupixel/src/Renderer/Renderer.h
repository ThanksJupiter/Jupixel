#pragma once

#include <glad/glad.h>
#include "glm/glm.hpp"

struct Texture2D;
struct RenderComponent;

struct RenderData
{
	RenderData(GLuint _VA, GLuint _VB, GLuint _IB, GLuint _ShaderID)
		: Quad_VA(_VA), Quad_VB(_VB), Quad_IB(_IB), ShaderID(_ShaderID) {}

	GLuint Quad_VA, Quad_VB, Quad_IB;
	GLuint ShaderID;
};

struct RenderObject
{
	RenderComponent* renderComponent = nullptr;
	GLuint ShaderID = 0;
	glm::vec2 Position = glm::vec2(0.0f);
	glm::vec4 Color = glm::vec4(1.0f);
	glm::vec2 Scale = glm::vec2(1.0f);
};

bool init_renderer();
void quit_renderer();

void begin_scene(GLuint ShaderID);
void end_scene();
void clear();
void render();

void queue_quad_for_rendering(
	glm::vec2& position = glm::vec2(0.0f),
	glm::vec4& color = glm::vec4(1.0f),
	glm::vec2& scale = glm::vec2(1.0f));

void queue_quad_for_rendering(RenderComponent* renderComponent);

void queue_GUI_quad_for_rendering(
	glm::vec2& position = glm::vec2(0.0f),
	glm::vec4& color = glm::vec4(1.0f),
	glm::vec3& scale = glm::vec3(1.0f));

void render_quad(
	glm::vec2& position = glm::vec2(0.0f),
	glm::vec4& color = glm::vec4(1.0f),
	glm::vec2& scale = glm::vec2(1.0f));

void render_quad(
	glm::vec3& position = glm::vec3(0.0f),
	glm::vec4& color = glm::vec4(1.0f),
	glm::vec3& scale = glm::vec3(1.0f));

void render_quad(
	Texture2D& texture,
	glm::vec2& position = glm::vec2(0.0f),
	glm::vec2& scale = glm::vec2(1.0f),
	glm::vec4& color = glm::vec4(1.0f));

void render_quad(
	Texture2D& texture,
	glm::vec3& position = glm::vec3(0.0f),
	glm::vec3& scale = glm::vec3(1.0f),
	glm::vec4& color = glm::vec4(1.0f));

void render_quad_outline(
	glm::vec2& position = glm::vec2(0.0f),
	glm::vec2& scale = glm::vec2(1.0f),
	glm::vec4& color = glm::vec4(1.0f));

void render_quad_outline(
	glm::vec3& position = glm::vec3(0.0f),
	glm::vec3& scale = glm::vec3(1.0f),
	glm::vec4& color = glm::vec4(1.0f));

void render_quad(RenderComponent* renderComponent);
