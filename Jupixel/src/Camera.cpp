#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera* camera = new Camera();

void camera_init()
{
	camera->m_AspectRatio = 16.0f / 9.0f;

	camera->m_ProjectionMatrix = glm::ortho(
		-camera->m_AspectRatio * camera->m_ZoomLevel,
		camera->m_AspectRatio * camera->m_ZoomLevel,
		-camera->m_ZoomLevel,
		camera->m_ZoomLevel,
		-1.0f, 1.0f);

	camera->m_ViewMatrix = glm::mat4(1.0f);
	camera->m_ViewProjectionMatrix = camera->m_ProjectionMatrix * camera->m_ViewMatrix;

	recalculate_view_matrix();
}

void set_position(glm::vec3& position)
{
	camera->m_Position = position;
	recalculate_view_matrix();
}

void set_rotation(float rotation)
{
	camera->m_Rotation = rotation;
	recalculate_view_matrix();
}

void recalculate_view_matrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), camera->m_Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(camera->m_Rotation), glm::vec3(0, 0, 1));

	camera->m_ViewMatrix = glm::inverse(transform);
	camera->m_ViewProjectionMatrix = camera->m_ProjectionMatrix * camera->m_ViewMatrix;
}

const glm::mat4 get_view_projection_matrix()
{
	return camera->m_ViewProjectionMatrix;
}
