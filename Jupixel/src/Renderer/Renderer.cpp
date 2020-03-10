#include "Renderer.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

RenderData* renderData = new RenderData();

// TODO what is this max 10 render things pls
RenderObject gameRenderQueue[10];
RenderObject GUIRenderQueue[10];

int gameRenderQueueIndex = 0;
int GUIRenderQueueIndex = 0;

bool init_renderer()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderData->ShaderID = compile_shader_program_from_text_files(
		"assets/shaders/vertex.glsl",
		"assets/shaders/fragment.glsl"
	);

	glCreateVertexArrays(1, &renderData->Quad_VA);
	glBindVertexArray(renderData->Quad_VA);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	glCreateBuffers(1, &renderData->Quad_VB);
	glBindBuffer(GL_ARRAY_BUFFER, renderData->Quad_VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
	glCreateBuffers(1, &renderData->Quad_IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderData->Quad_IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	camera_init();

	return true;
}

void quit_renderer()
{
	glDeleteVertexArrays(1, &renderData->Quad_VA);
	glDeleteBuffers(1, &renderData->Quad_VB);
	glDeleteBuffers(1, &renderData->Quad_IB);
	delete renderData;
}

void clear()
{
	glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void render()
{
	for (int i = 0; i < gameRenderQueueIndex; i++)
	{
		RenderObject& o = gameRenderQueue[i];

		render_quad(o.Position, o.Color, o.Scale);
	}
	gameRenderQueueIndex = 0;

	for (int i = 0; i < GUIRenderQueueIndex; i++)
	{
		RenderObject& o = GUIRenderQueue[i];

		render_quad(o.Position, o.Color, o.Scale);
	}
	GUIRenderQueueIndex = 0;
}

void queue_quad_for_rendering(glm::vec2& position, glm::vec4& color, glm::vec2& scale)
{
	RenderObject& o = gameRenderQueue[gameRenderQueueIndex];
	o.Position = position;
	o.Color = color;
	o.Scale = scale;
	gameRenderQueueIndex++;
}

void queue_GUI_quad_for_rendering(glm::vec2& position /*= glm::vec2(0.0f)*/, glm::vec4& color /*= glm::vec4(1.0f)*/, glm::vec3& scale /*= glm::vec3(1.0f)*/)
{
	RenderObject& o = GUIRenderQueue[GUIRenderQueueIndex];
	o.Position = position;
	o.Color = color;
	o.Scale = scale;
	GUIRenderQueueIndex++;
}

void begin_scene()
{
	glUseProgram(renderData->ShaderID);

	int location = glGetUniformLocation(renderData->ShaderID, "u_ViewProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(get_view_projection_matrix()));
}

void render_quad(glm::vec3& position /*= glm::vec2(0.0f)*/, glm::vec4& color /*= glm::vec4(1.0f)*/, glm::vec3& scale /*= glm::vec3(1.0f)*/)
{
	int location = glGetUniformLocation(renderData->ShaderID, "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(color));

	location = glGetUniformLocation(renderData->ShaderID, "u_Transform");
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 1.0f)) * glm::scale(glm::mat4(1.0f), scale);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(renderData->Quad_VA);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void render_quad(glm::vec2& position /*= glm::vec2(0.0f)*/, glm::vec4& color /*= glm::vec4(1.0f)*/, glm::vec2& scale /*= glm::vec2(1.0f)*/)
{
	glm::vec3 v3Pos = glm::vec3(position.x, position.y, 0.0f);
	glm::vec3 v3Scale = glm::vec3(scale.x, scale.y, 1.0f);

	render_quad(v3Pos, color, v3Scale);
}
