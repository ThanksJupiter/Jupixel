#include "Renderer.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

RenderData* renderData = new RenderData();

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

void render_quad()
{
	glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderData->ShaderID);

	int location = glGetUniformLocation(renderData->ShaderID, "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(glm::vec4(0.45f, 0.75f, 0.2f, 1.0f)));

	location = glGetUniformLocation(renderData->ShaderID, "u_ViewProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(get_view_projection_matrix()));

	glBindVertexArray(renderData->Quad_VA);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void render_quad(glm::vec2& position)
{
	glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderData->ShaderID);

	int location = glGetUniformLocation(renderData->ShaderID, "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(glm::vec4(0.66f, 0.3f, 0.2f, 1.0f)));

	location = glGetUniformLocation(renderData->ShaderID, "u_ViewProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(get_view_projection_matrix()));

	location = glGetUniformLocation(renderData->ShaderID, "u_Transform");
	glm::vec3 v3pos = { position.x, position.y, 0.0f };
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), v3pos) * glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f});
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(renderData->Quad_VA);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}