#pragma once

#include <glm.hpp>
#include <gtx\transform.hpp>
#include "component.h"

namespace Engine { namespace Components {

	class Transform : public Component
	{
	protected:
		//! Private glm::vec3 variable.
		/*! Holds the position of the object */
		glm::vec3 p_Position;
		//! Private glm::vec3 variable.
		/*! Holds the eular rotation of the object */
		glm::vec3 p_EularRotation;
		//! Private glm::vec3 variable.
		/*! Holds the size of the object */
		glm::vec3 p_Scale;
	public:
		Transform() {}
		Transform(glm::vec3 pos, glm::vec3 eularRot, glm::vec3 scale) : p_Position(pos), p_EularRotation(eularRot), p_Scale(scale) {}
		~Transform() {}

		void Update(float deltaTime) override {};
		void Message(const char* message) override {};

		//! The get transform data member functions
		/*!
		Returns infomation regarding the transform variables (position, size and rotation)
		*/
		inline const glm::vec3& getPosition() const { return p_Position; }
		inline const glm::vec3& getSize() const { return p_Scale; }
		inline const glm::vec3& getRotation() const { return p_EularRotation; }

		//! The set transform data member functions
		/*!
		Sets infomation regarding the transform variables (position, size and rotation)
		*/
		inline const void setPosition(glm::vec3& newPos) { p_Position = newPos; }
		inline const void setSize(glm::vec3& newSize) { p_Scale = newSize; }
		inline const void setRotation(glm::vec3& newRot) { p_EularRotation = newRot; }

		//! General transformation functions
		/*!
		Three functions for doing general transformations (Translate, Rotate and Scale)
		*/
		inline const void Rotate(glm::vec3 amount) { p_EularRotation += amount; }
		inline const void Translate(glm::vec3 amount) { p_Position += amount; }
		inline const void Scale(glm::vec3 amount) { p_Scale += amount; }

		inline const glm::mat4 getMatrix() { return  glm::translate(p_Position) * glm::rotate(1.0f, p_EularRotation) * glm::scale(p_Scale); }
	};
} 
}