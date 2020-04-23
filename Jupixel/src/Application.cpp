#include "Application.h"
#include "Renderer/Renderer.h"
#include "Camera.h"

#include "Key.h"

#include <stdio.h>

#include "World.h"
#include "SkeletonAnimations.h"
#include "UserInput.h"
#include "Systems/NetworkSystem.h"

#include "ImGui/GUILayer.h"
#include "Player.h"

bool isRunning = true;
bool is_setup = true;
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

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hellskel", NULL, NULL);
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

	net_init();

	init_renderer();
	on_window_resize(SCREEN_WIDTH, SCREEN_HEIGHT);
	init_GUI();

	load_skeleton_sprites();
	setup_world();

	return success;
}

void quit_application()
{
	quit_GUI();
	net_quit();
	quit_renderer();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void run_application()
{
	while (is_setup && isRunning)
	{
		update_gamepad_state(0);

		if (is_key_pressed(KeyCode::V) || is_button_down(0, GLFW_GAMEPAD_BUTTON_X))
		{
			printf("Server setup...");
			get_world()->player_one->Network = new NetworkComponent();
			net_init_server_component(*get_world()->player_one->Network);
			is_setup = false;
		}

		if (is_key_pressed(KeyCode::C) || is_button_down(0, GLFW_GAMEPAD_BUTTON_Y))
		{
			printf("Client setup...");
			get_world()->player_one->Network = new NetworkComponent();
			net_init_client_component(*get_world()->player_one->Network);
			is_setup = false;
		}
		if (is_key_pressed(KeyCode::B) || is_button_down(0, GLFW_GAMEPAD_BUTTON_A))
		{
			printf("No network");
			is_setup = false;
		}

		// so no need to press b to begin
		is_setup = false;

		glfwPollEvents();

		clear();
		render();

		glfwSwapBuffers(window);
	}

	while (isRunning)
	{
		float time = (float)glfwGetTime();
		deltaTime = time - lastFrameTime;
		//deltaTime = glm::clamp(deltaTime, 0.01f, 0.1f);
		lastFrameTime = time;

		// TODO figure out delta times
		// only update with fixed timestep every so often based on how fast program is updated
		float fixed_dt = 1/60.0f;

		glfwPollEvents();
		clear();
		
		update_world(deltaTime, fixed_dt);
		render();

		glfwSwapBuffers(window);

		if (is_key_pressed(KeyCode::Escape))
		{
			isRunning = false;
		}
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
