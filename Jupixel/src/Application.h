#pragma once

#include "GLFW/glfw3.h"

void error_callback(int error, const char* description);
bool init_application();
void quit_application();
void run_application();

GLFWwindow* get_window();

// window functions
void on_window_resize(int width, int height);
void on_zoom(int xOffset, int yOffset);
