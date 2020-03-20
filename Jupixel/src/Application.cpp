#include "Application.h"
#include "Renderer/Renderer.h"
#include "Camera.h"

#include "Key.h"

#include <stdio.h>

#include "ECS/ECS.h"
#include "ECS/ComponentLists.h"

bool isRunning = true;
float lastFrameTime = 0.0f;
float deltaTime = 0.0f;

GLFWwindow* window;

void error_callback(int error, const char* description)
{
	printf("Error: %s\n", description);
}

bool init_application()
{
	bool success = glfwInit();

	if (!success)
	{
		printf("Unable to initialize glfw");
		return false;
	}
	glfwSetErrorCallback(error_callback);

	window = glfwCreateWindow(640, 480, "Calcium Clash", NULL, NULL);
	if (!window)
	{
		printf("Unable to create window");
		return false;
	}

	glfwMakeContextCurrent(window);

	success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	if (!success)
	{
		printf("Unable to initialize glad");
		return false;
	}

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{
		on_window_resize(width, height);
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		on_zoom(xOffset, yOffset);
	});

	glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
	{
		isRunning = false;
	});

	init_renderer();

	return success;
}

void quit()
{
	quit_renderer();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void run()
{
	ComponentLists* components = new ComponentLists[10];

	create_entity(components, glm::vec4(0.1f, 1.0f, 0.1f, 1.0f));
	create_entity(components, glm::vec4(0.1f, 0.1f, 1.0f, 1.0f));

	while (isRunning)
	{
		float time = (float)glfwGetTime();
		deltaTime = time - lastFrameTime;
		lastFrameTime = time;

		glfwPollEvents();

		clear();
		ecs_update(components, deltaTime);

		render();

		glfwSwapBuffers(window);
	}
}

GLFWwindow* get_window()
{
	return window;
}

void on_window_resize(int width, int height)
{
	set_aspect_ratio((float)width / (float)height);
	set_projection(get_aspect_ratio(), get_zoom_level());

	glViewport(0, 0, width, height);
}

void on_zoom(int xOffset, int yOffset)
{
	zoom(yOffset * 0.25f);
	set_projection(get_aspect_ratio(), get_zoom_level());
}
