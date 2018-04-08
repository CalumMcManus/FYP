#pragma once
#include "window.h"



namespace Engine {

	class Project;
	//! GLFWEngine 
	/*!
	GLFWEngine class. Contains the programs main loop updating the project and window.
	*/
	class GLFWEngine
	{
	public:
		//! GLFWEngine Contructor
		/*!
		Calls fucntion for inital set up of a project
		\param projectName Name displayed on the window
		\param isNew True if a new project
		*/
		GLFWEngine(const char* projectName, bool isNew);
		//! GLFWEngine Destructor.
		/*!
		Cleans up memory in the class
		*/
		~GLFWEngine();
		//! The Run member function
		/*!
		Main program loop. Clears window, draws UI and updates project
		*/
		void Run();
		void Resize();
		//! Public Window pointer.
		/*! Pointer to the Window class */
		graphics::Window* m_Window;
		//! The GetProject member function
		/*!
		Returns pointer to the project
		*/
		Project* GetProject() { return m_Project; }
	private:
		//! Private Project pointer.
		/*! Pointer to the Project class */
		Project* m_Project;

	};
}