#pragma once
#include "window.h"



namespace Engine {

	class Project;
	class GLFWEngine
	{
	public:
		GLFWEngine(const char* projectName, bool isNew);
		~GLFWEngine();

		void Run();
		void Resize();
		graphics::Window* m_Window;
	private:
		
		Project* m_Project;

	};
}