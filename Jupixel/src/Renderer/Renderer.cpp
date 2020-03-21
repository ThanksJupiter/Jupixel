#include "Renderer.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glad/glad.h"
#include "Camera.h"
#include "Texture2D.h"
#include "ECS/Components/Components.h"

RenderData* textureRenderData = nullptr;
RenderData* flatColorRenderData = nullptr;
RenderData* outlineRenderData = nullptr;

// TODO what is this max 10 render things pls
RenderObject gameRenderQueue[10];
RenderObject GUIRenderQueue[10];

GLuint currentShaderID = 0;

int gameRenderQueueIndex = 0;
int GUIRenderQueueIndex = 0;

bool init_renderer()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	unsigned int VBO, VAO, IBO;
	unsigned int shaderID;
	unsigned int textureID;

	{
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

		textureRenderData = new RenderData(VAO, VBO, IBO,
										   compile_shader_program_from_text_files(
										   "assets/shaders/texture_vertex.glsl",
										   "assets/shaders/texture_fragment.glsl"));
	}
	{
		float vertices[] = {
			// positions       
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		flatColorRenderData = new RenderData(VAO, VBO, IBO,
											 compile_shader_program_from_text_files(
											 "assets/shaders/vertex.glsl",
											 "assets/shaders/fragment.glsl"));
	}

	{
		float vertices[] = {
			// positions       
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		unsigned int indices[] = {
			0, 1,
			1, 2,
			2, 3,
			3, 0
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		outlineRenderData = new RenderData(VAO, VBO, IBO,
											 compile_shader_program_from_text_files(
											 "assets/shaders/vertex.glsl",
											 "assets/shaders/fragment.glsl"));
	}

	camera_init();

	return true;
}

void quit_renderer()
{
	delete textureRenderData;
	delete flatColorRenderData;
}

void clear()
{
	glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void render()
{
	begin_scene(textureRenderData->ShaderID);
	for (int i = 0; i < gameRenderQueueIndex; i++)
	{
		RenderObject& o = gameRenderQueue[i];

		render_quad(o.renderComponent);
	}
	gameRenderQueueIndex = 0;

	begin_scene(flatColorRenderData->ShaderID);
	for (int i = 0; i < GUIRenderQueueIndex; i++)
	{
		RenderObject& o = GUIRenderQueue[i];

		render_quad(o.Position, o.Color, o.Scale);
	}
	GUIRenderQueueIndex = 0;
}

void update_texture_coordinates(int index)
{
	// TODO calculate based on sprite sheet
	int rows = (int)64/32;

	float uv_x = (float)(index % rows) / rows;
	float uv_y = (float)(index / rows) / rows;

	glm::vec2 topRight    = glm::vec2(uv_x + 1.0f / rows, uv_y);
	glm::vec2 bottomRight = glm::vec2(uv_x + 1.0f / rows, uv_y + 1.0f / rows);
	glm::vec2 bottomLeft  = glm::vec2(uv_x,							 uv_y + 1.0f / rows);
	glm::vec2 topLeft     = glm::vec2(uv_x,							 uv_y);

	float vertices[] = {
		// positions        // texture coords
		 0.5f,  0.5f, 0.0f, topRight.x,    topRight.y,
		 0.5f, -0.5f, 0.0f, bottomRight.x, bottomRight.y,
		-0.5f, -0.5f, 0.0f, bottomLeft.x,  bottomLeft.y,
		-0.5f,  0.5f, 0.0f, topLeft.x,     topLeft.y
	};

	glBindBuffer(GL_ARRAY_BUFFER, textureRenderData->Quad_VB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

void queue_quad_for_rendering(glm::vec2& position, glm::vec4& color, glm::vec2& scale)
{
	RenderObject& o = gameRenderQueue[gameRenderQueueIndex];
	o.Position = position;
	o.Color = color;
	o.Scale = scale;
	gameRenderQueueIndex++;
}

void queue_quad_for_rendering(RenderComponent* renderComponent)
{
	RenderObject& o = gameRenderQueue[gameRenderQueueIndex];
	o.renderComponent = renderComponent;
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

void begin_scene(GLuint shaderID)
{
	currentShaderID = shaderID;
	glUseProgram(currentShaderID);
	int location = glGetUniformLocation(shaderID, "u_ViewProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(get_view_projection_matrix()));
}

void end_scene()
{
	currentShaderID = 0;
}

void render_quad_outline(glm::vec2& position /*= glm::vec2(0.0f)*/, glm::vec2& scale /*= glm::vec2(1.0f)*/, glm::vec4& color /*= glm::vec4(1.0f)*/)
{
	glm::vec3 v3Pos = glm::vec3(position.x, position.y, 0.0f);
	glm::vec3 v3Scale = glm::vec3(scale.x, scale.y, 0.0f);

	render_quad_outline(v3Pos, v3Scale, color);
}

void render_quad_outline(glm::vec3& position /*= glm::vec3(0.0f)*/, glm::vec3& scale /*= glm::vec3(1.0f)*/, glm::vec4& color /*= glm::vec4(1.0f)*/)
{
	int location = glGetUniformLocation(currentShaderID, "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(color));

	location = glGetUniformLocation(currentShaderID, "u_Transform");
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f)) * glm::scale(glm::mat4(1.0f), scale);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(outlineRenderData->Quad_VA);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
}

void render_quad(glm::vec3& position /*= glm::vec2(0.0f)*/, glm::vec4& color /*= glm::vec4(1.0f)*/, glm::vec3& scale /*= glm::vec3(1.0f)*/)
{
	int location = glGetUniformLocation(currentShaderID, "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(color));

	location = glGetUniformLocation(currentShaderID, "u_Transform");
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f)) * glm::scale(glm::mat4(1.0f), scale);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(flatColorRenderData->Quad_VA);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void render_quad(glm::vec2& position /*= glm::vec2(0.0f)*/, glm::vec4& color /*= glm::vec4(1.0f)*/, glm::vec2& scale /*= glm::vec2(1.0f)*/)
{
	glm::vec3 v3Pos = glm::vec3(position.x, position.y, 0.0f);
	glm::vec3 v3Scale = glm::vec3(scale.x, scale.y, 0.0f);

	render_quad(v3Pos, color, v3Scale);
}

void render_quad(Texture2D& texture, glm::vec2& position /*= glm::vec2(0.0f)*/, glm::vec2& scale /*= glm::vec2(1.0f)*/, glm::vec4& color /*= glm::vec4(1.0f)*/)
{
	glm::vec3 v3Pos = glm::vec3(position.x, position.y, 0.0f);
	glm::vec3 v3Scale = glm::vec3(scale.x, scale.y, 0.0f);

	render_quad(texture, v3Pos, v3Scale, color);
}

void render_quad(Texture2D& texture, glm::vec3& position /*= glm::vec3(0.0f)*/, glm::vec3& scale /*= glm::vec3(1.0f)*/, glm::vec4& color /*= glm::vec4(1.0f)*/)
{
	int location = glGetUniformLocation(currentShaderID, "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(color));

	bind_texture(texture.ID);
	location = glGetUniformLocation(currentShaderID, "u_Texture");
	// TODO what is going on with this zero
	glUniform1i(location, 0);

	location = glGetUniformLocation(currentShaderID, "u_Transform");
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f)) * glm::scale(glm::mat4(1.0f), scale);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(textureRenderData->Quad_VA);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void render_quad(RenderComponent* renderComponent)
{
	glm::vec3 v3Pos = glm::vec3(renderComponent->x, renderComponent->y, 0.0f);
	glm::vec3 v3Scale = glm::vec3(renderComponent->Scale.x, renderComponent->Scale.y, 1.0f);

	renderComponent->texture == nullptr ?
		render_quad(v3Pos, renderComponent->Color, v3Scale) :
		render_quad(*renderComponent->texture, v3Pos, v3Scale, renderComponent->Color);
}
