#include "jppch.h"

#include "Camera.h"

Camera* camera = new Camera();

void camera_init()
{
	camera->AspectRatio = 640.0f / 480.0f;

	set_projection(camera->AspectRatio, camera->ZoomLevel);
	recalculate_view_matrix();
}

void set_position(glm::vec3& position)
{
	camera->Position = position;
	recalculate_view_matrix();
}

void set_rotation(float rotation)
{
	camera->Rotation = rotation;
	recalculate_view_matrix();
}

void set_projection(float aspectRatio, float zoomLevel)
{
	camera->AspectRatio = aspectRatio;
	camera->ZoomLevel = zoomLevel;

	camera->ProjectionMatrix = glm::ortho(
		-camera->AspectRatio * camera->ZoomLevel,
		camera->AspectRatio * camera->ZoomLevel,
		-camera->ZoomLevel,
		camera->ZoomLevel,
		-1.0f, 1.0f);

	camera->ViewMatrix = glm::mat4(1.0f);
	camera->ViewProjectionMatrix = camera->ProjectionMatrix * camera->ViewMatrix;

	recalculate_view_matrix();
}

void zoom(float amount)
{
	camera->ZoomLevel -= amount;
	camera->ZoomLevel = std::max(camera->ZoomLevel, 0.25f);
}

void recalculate_view_matrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), camera->Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(camera->Rotation), glm::vec3(0, 0, 1));

	camera->ViewMatrix = glm::inverse(transform);
	camera->ViewProjectionMatrix = camera->ProjectionMatrix * camera->ViewMatrix;
}

const glm::mat4 get_view_projection_matrix()
{
	return camera->ViewProjectionMatrix;
}

void set_aspect_ratio(float value)
{
	camera->AspectRatio = value;
}

void set_zoom_level(float value)
{
	camera->ZoomLevel = value;
}

float get_aspect_ratio()
{
	return camera->AspectRatio;
}

float get_zoom_level()
{
	return camera->ZoomLevel;
}
