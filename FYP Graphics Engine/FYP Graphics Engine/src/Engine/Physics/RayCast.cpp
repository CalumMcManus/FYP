#include <Engine\Physics\RayCast.h>

glm::vec3 Engine::Physics::RayCast::ViewToWorldSpace(glm::vec2 screenPos, glm::mat4 projection, glm::mat4 view, graphics::Window* window)
{
	//Screen to Normalised Device Coordinates
	float x = (2.0f * screenPos.x) / window->getWidth() - 1.0f;
	float y = 1.0f - (2.0f * screenPos.y) / window->getHeight();
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	//Normalised Device Coordinates to 4d Homogeneous Clip Coordinates
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

	//4d Homogeneous Clip Coordinates to Eye (Camera) Coordinates
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	//Eye (Camera) Coordinates to 3d World Coordinates
	glm::vec3 ray_wor = glm::inverse(view) * ray_eye;
	ray_wor = glm::normalize(ray_wor);
	return ray_wor;
}

std::vector<glm::vec3>  Engine::Physics::RayCast::CastRay(glm::vec3 origin, glm::vec2 screenPos, float distance, glm::mat4 projection, glm::mat4 view, graphics::Window * window)
{
	glm::vec3 vDir = ViewToWorldSpace(screenPos, projection, view, window);
	float t = 1;
	int iterations = 100;
	float betweenDist = distance / iterations;
	//Rays use O+dt
	glm::vec3 rayPoint = origin + vDir*t;
	std::vector<glm::vec3> ray;


	for (int i = 0; i < iterations; i++)
	{
		ray.push_back(rayPoint);
		t++;
		rayPoint = origin + vDir*t;
	}

	return ray;
}
