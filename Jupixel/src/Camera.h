#pragma once

#include "glm/glm.hpp"

struct Camera
{
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ViewProjectionMatrix;

	glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
	float Rotation = 0.0f;

	float AspectRatio;
	float ZoomLevel = 1.0f;
};

void camera_init();
void set_position(glm::vec3& position);
void set_rotation(float rotation);
void set_projection(float aspectRatio, float zoomLevel);

void zoom(float amount);

void recalculate_view_matrix();

const glm::mat4 get_view_projection_matrix();

void set_aspect_ratio(float value);
void set_zoom_level(float value);

float get_aspect_ratio();
float get_zoom_level();
