#include "Application.h"
#include "Renderer/Renderer.h"

#include "GLFW/glfw3.h"
#include <stdio.h>

bool isRunning = true;
float timestep = 0.0f;

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
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	if (!success)
	{
		printf("Unable to initialize glad");
		glfwTerminate();
		return false;
	}

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
	while (isRunning)
	{
		render_quad();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
