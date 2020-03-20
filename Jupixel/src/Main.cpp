#include "Application.h"
#include <stdio.h>

/*
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <stb_image.h>
#include <string>

#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>*/

/*
GLFWwindow* tmp_window;

bool tmp_is_running = true;

unsigned int tmp_VB, tmp_VA, tmp_IB;

unsigned int tmp_texture_ID;
unsigned int tmp_shader_ID;*/

/*
void tmp_load_texture()
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = data = stbi_load("assets/textures/skel.png", &width, &height, &channels, 0);
	
	if (data == nullptr)
	{
		printf("unable to load texture");
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &tmp_texture_ID);
	glTextureStorage2D(tmp_texture_ID, 1, GL_RGBA8, width, height);

	glTextureParameteri(tmp_texture_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(tmp_texture_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(tmp_texture_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(tmp_texture_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTextureSubImage2D(tmp_texture_ID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

void tmp_load_shader()
{
	tmp_shader_ID = compile_shader_program_from_text_files(
		"assets/shaders/texture_vertex.glsl",
		"assets/shaders/texture_fragment.glsl");
}*/

int main(int argc, char** argv)
{
	bool success = init_application();

	if (!success)
	{
		printf("Unable to initialize :(");
		return -1;
	}

	run();

	quit();

	/*glfwInit();
	tmp_window = glfwCreateWindow(640, 480, "test", nullptr, nullptr);

	glfwMakeContextCurrent(tmp_window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	tmp_load_shader();
	tmp_load_texture();

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

	glGenVertexArrays(1, &tmp_VA);
	glGenBuffers(1, &tmp_VB);
	glGenBuffers(1, &tmp_IB);

	glBindVertexArray(tmp_VA);

	glBindBuffer(GL_ARRAY_BUFFER, tmp_VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmp_IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


	while (tmp_is_running)
	{
		glfwPollEvents();

		glClearColor(0.5f, 0.1f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(tmp_shader_ID);
		glBindTexture(GL_TEXTURE_2D, tmp_texture_ID);

		int location = glGetUniformLocation(tmp_shader_ID, "u_Color");
		glUniform4fv(location, 1, glm::value_ptr(glm::vec4(1.0f, 0.5f, 0.5f, 1.0f)));

		location = glGetUniformLocation(tmp_shader_ID, "u_Texture");
		glUniform1i(location, 0);

		glBindVertexArray(tmp_VA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(tmp_window);
	}*/

	return 0;
}
