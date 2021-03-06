#include "CameraController.h"

#include "glm/glm.hpp"

#include "Player.h"
#include "Components/TransformComponent.h"

#include "Renderer/Renderer.h"

const float x_clamp = 1.0f; // 0.5f is good hehe
const float y_offset = 1.0f;

void camera_update(Player* player_one, Player* player_two, float dt)
{
	glm::vec2 pos_one = player_one->Transform.Position;
	glm::vec2 pos_two = player_two->Transform.Position;

	glm::vec2 camera_center = (pos_one + ((pos_two - pos_one) * 0.5f));
	camera_center.y += y_offset;
	
	float zoom_level = glm::distance(pos_two, pos_one);
	zoom_level = glm::clamp(zoom_level, 1.5f, 3.0f);

	float cam_y_pos = glm::clamp(camera_center.y, -1.0f, zoom_level - 1.5f);
	float cam_x_pos = glm::clamp(camera_center.x, -x_clamp, x_clamp);

	glm::vec3 cam_pos = glm::vec3(cam_x_pos, cam_y_pos, 0.0f);

	set_position(cam_pos);
	set_zoom_level(zoom_level);

	set_projection(get_aspect_ratio(), get_zoom_level());
}
