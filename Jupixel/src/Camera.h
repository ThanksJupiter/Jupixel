#pragma once

#include "glm/glm.hpp"

struct Camera
{
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	float m_Rotation = 0.0f;

	float m_AspectRatio;
	float m_ZoomLevel = 1.0f;
};

void camera_init();
void set_position(glm::vec3& position);
void set_rotation(float rotation);
void recalculate_view_matrix();

const glm::mat4 get_view_projection_matrix();
