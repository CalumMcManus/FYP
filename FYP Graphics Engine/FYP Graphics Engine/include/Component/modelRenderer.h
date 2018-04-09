#pragma once
#include "component.h"
#include <Engine\Graphics\Model.h>
namespace Engine { namespace Components {
	//! ModelRenderer 
	/*!
	Holds all imfomation regarding an object's Model.
	Inherits from Component.
	Contained in the Engine::Components namespace.
	*/
	class ModelRenderer : public Component
	{
	public:
		//! ModelRenderer Contructor
		/*!
		Sets up the components model class and holds the file path
		\param modelPath File path of model
		*/
		ModelRenderer(const char* modelPath) 
		{ 
			m_Model = new graphics::Model(modelPath);
			m_Path = modelPath;
		};
		//! ModelRenderer Decontructor
		/*!
		Clears memory in class
		*/
		virtual ~ModelRenderer()
		{
			std::cout << "renderer delete" << std::endl;
			delete m_Model;
			m_Model = nullptr;
		};
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

		//! The getModel member function
		/*!
		Returns referance to the components model class
		*/
		graphics::Model& getModel() { return *m_Model; }
		//! The Path member function
		/*!
		Returns models file path as a string
		*/
		const std::string Path() const { return m_Path; }
	private:
		//! Private Model pointer.
		/*! Component Model class */
		graphics::Model* m_Model;
		//! Private string variable.
		/*! Path to the components model */
		std::string m_Path;


	};

} 
}