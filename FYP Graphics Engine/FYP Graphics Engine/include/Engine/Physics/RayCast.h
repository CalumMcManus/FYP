#pragma once
#include <glm.hpp>
#include <Engine\window.h>
#include <vector>
namespace Engine {
	namespace Physics {

		class RayCast
		{
		public:
			static glm::vec3 ViewToWorldSpace(glm::vec2 screenPos, glm::mat4 projection, glm::mat4 view, graphics::Window* window);
			static std::vector<glm::vec3> CastRay(glm::vec3 origin, glm::vec2 screenPos, float distance, glm::mat4 projection, glm::mat4 view, graphics::Window* window);
		};
} }
