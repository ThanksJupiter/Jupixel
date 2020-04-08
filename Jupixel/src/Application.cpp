#include "Application.h"
#include "Renderer/Renderer.h"
#include "Camera.h"

#include "Key.h"

#include <stdio.h>

#include "World.h"
#include "SkeletonAnimations.h"
#include "UserInput.h"

#include "ImGui/GUILayer.h"

bool isRunning = true;
float lastFrameTime = 0.0f;
float deltaTime = 0.0f;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 480;

float window_width = 0.0f;
float window_height = 0.0f;

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

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Calcium Clash", NULL, NULL);
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
	on_window_resize(SCREEN_WIDTH, SCREEN_HEIGHT);
	init_GUI();

	return success;
}

void quit_application()
{
	quit_GUI();
	quit_renderer();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void run_application()
{
	load_skeleton_sprites();

	setup_world();

	while (isRunning)
	{
		float time = (float)glfwGetTime();
		deltaTime = time - lastFrameTime;
		//deltaTime = glm::clamp(deltaTime, 0.01f, 0.1f);
		lastFrameTime = time;

		glfwPollEvents();
		clear();
		
		update_world(deltaTime * get_time_scale());
		render();

		glfwSwapBuffers(window);
	}
}

GLFWwindow* get_window()
{
	return window;
}

float get_window_width()
{
	return window_width;
}

float get_window_height()
{
	return window_height;
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
