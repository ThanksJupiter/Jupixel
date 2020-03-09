#pragma once

#include "GLFW/glfw3.h"

void error_callback(int error, const char* description);
bool init_application();
void quit();
void run();

GLFWwindow* get_window();

void on_window_resize(int width, int height);
void on_zoom(int xOffset, int yOffset);
