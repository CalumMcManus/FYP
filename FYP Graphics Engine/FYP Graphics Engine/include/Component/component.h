#pragma once

namespace Engine { namespace Components {
	//! Component 
	/*!
	Base class for all GameObject Components.
	Contained in the Engine::Components namespace.
	*/
	class Component
	{
	public:
		//! The Update member function
		/*!
		Base function for Updating components each frame
		\param deltaTime float Time passed since last frame
		*/
		virtual void Update(float deltaTime) = 0;
		//! The Message member function
		/*!
		Base function for Sending messages to components
		\param message const char pointer Message
		*/
		virtual void Message(const char* message) = 0;
		virtual ~Component() {};
	};
} 
}

