#pragma once
#include <glm.hpp>
#include <Engine\window.h>
#include <vector>
namespace Engine {
	namespace Physics {
		//! RayCast 
		/*!
		RayCast used for casting rays for object picking.
		Contained in the Engine::Physics namespace.
		*/
		class RayCast
		{
		public:
			//! The ViewToWorldSpace static member function
			/*!
			Converts a vec2 screen position into vec3 world position
			\param screenPos glm::vec2 screen position
			\param projection glm::mat4 projection matrixs
			\param view glm::mat4 view matrix
			\param window Window pointer
			*/
			static glm::vec3 ViewToWorldSpace(glm::vec2 screenPos, glm::mat4 projection, glm::mat4 view, graphics::Window* window);
			//! The CastRay static member function
			/*!
			Returns an array of glm::vec3 positions in a stright line in a given direction
			\param origin glm::vec3 world position
			\param screenPos glm::vec2 screen position
			\param distance float ray length
			\param view glm::mat4 view matrix
			\param window Window pointer
			*/
			static std::vector<glm::vec3> CastRay(glm::vec3 origin, glm::vec2 screenPos, float distance, glm::mat4 projection, glm::mat4 view, graphics::Window* window);
		};
} }
