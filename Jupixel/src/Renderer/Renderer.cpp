#include "Renderer.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glad/glad.h"
#include "Camera.h"
#include "Texture2D.h"

RenderData* renderData = new RenderData();

unsigned int VBO, VAO, IBO;
unsigned int shaderID;
unsigned int textureID;

// TODO what is this max 10 render things pls
RenderObject gameRenderQueue[10];
RenderObject GUIRenderQueue[10];

int gameRenderQueueIndex = 0;
int GUIRenderQueueIndex = 0;

bool init_renderer()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float vertices[] = {
		// positions        // texture coords
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
	   0, 1, 2, // first triangle
	   2, 3, 0  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	shaderID = compile_shader_program_from_text_files(
		"assets/shaders/texture_vertex.glsl",
		"assets/shaders/texture_fragment.glsl");

	textureID = load_texture("assets/textures/skel.png");

	camera_init();

	return true;
}

void quit_renderer()
{
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
	glUseProgram(shaderID);

	int location = glGetUniformLocation(shaderID, "u_ViewProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(get_view_projection_matrix()));
}

void render_quad(glm::vec3& position /*= glm::vec2(0.0f)*/, glm::vec4& color /*= glm::vec4(1.0f)*/, glm::vec3& scale /*= glm::vec3(1.0f)*/)
{
	int location = glGetUniformLocation(shaderID, "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(color));

	glBindTextureUnit(0, textureID);
	location = glGetUniformLocation(shaderID, "u_Texture");
	glUniform1i(location, 0);

	location = glGetUniformLocation(shaderID, "u_Transform");
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 1.0f)) * glm::scale(glm::mat4(1.0f), scale);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void render_quad(glm::vec2& position /*= glm::vec2(0.0f)*/, glm::vec4& color /*= glm::vec4(1.0f)*/, glm::vec2& scale /*= glm::vec2(1.0f)*/)
{
	glm::vec3 v3Pos = glm::vec3(position.x, position.y, 0.0f);
	glm::vec3 v3Scale = glm::vec3(scale.x, scale.y, 1.0f);

	render_quad(v3Pos, color, v3Scale);
}
