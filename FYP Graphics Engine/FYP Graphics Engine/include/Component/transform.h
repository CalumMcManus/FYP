#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx\transform.hpp>
#include <gtc/type_ptr.hpp>
#include <mat4x4.hpp>
#include "component.h"

namespace Engine { namespace Components {
	//! Transform 
	/*!
	Holds all imfomation regarding a GameObjects transform (position, rotation and scale)
	Inherits from Component.
	Contained in the Engine::Components namespace.
	*/
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
		//! Transform Contructor
		/*!
		Empty Contructor
		*/
		Transform() {}
		//! Transform Contructor
		/*!
		Sets up the Transforms three variables
		\param pos glm::vec3 Transforms Prosition
		\param eularRot glm::vec3 Transforms Rotation
		\param scale glm::vec3 Transforms Scale
		*/
		Transform(glm::vec3 pos, glm::vec3 eularRot, glm::vec3 scale) : p_Position(pos), p_EularRotation(eularRot), p_Scale(scale) {}
		//! Transform Destructor.
		/*!
		Cleans up memory in the class
		*/
		~Transform() {}
		//! The Update member function
		/*!
		Function for Updating components each frame
		\param deltaTime float Time passed since last frame
		*/
		void Update(float deltaTime) override {};
		//! The Message member function
		/*!
		Function for Sending messages to components
		\param message const char pointer Message
		*/
		void Message(const char* message) override {};

		
		/** @defgroup The get transform data member functions
		*  Returns infomation regarding the transform variables (position, size and rotation)
		*  @{
		*/
		inline const glm::vec3& getPosition() const { return p_Position; }
		inline const glm::vec3& getSize() const { return p_Scale; }
		inline const glm::vec3& getRotation() const { return p_EularRotation; }
		/** @} */ //

		/** @defgroup The set transform data member functions
		*  Sets infomation regarding the transform variables (position, size and rotation)
		*  @{
		*/
		inline const void setPosition(glm::vec3& newPos) { p_Position = newPos; }
		inline const void setSize(glm::vec3& newSize) { p_Scale = newSize; }
		inline const void setRotation(glm::vec3& newRot) { p_EularRotation = newRot; }
		/** @} */ //

		/** @defgroup General transformation functions
		*  Three functions for doing general transformations (Translate, Rotate and Scale)
		*  @{
		*/
		inline const void Rotate(glm::vec3 amount) { p_EularRotation += amount; }
		inline const void Translate(glm::vec3 amount) { p_Position += amount; }
		inline const void Scale(glm::vec3 amount) { p_Scale += amount; }
		/** @} */ //

		//! The getMatrix member function
		/*!
		Returns the transform model matrix
		*/
		inline const glm::mat4 getMatrix() { return  glm::translate(p_Position) * glm::mat4_cast(glm::quat(glm::radians(p_EularRotation))) * glm::scale(p_Scale); }
	};
} 
}