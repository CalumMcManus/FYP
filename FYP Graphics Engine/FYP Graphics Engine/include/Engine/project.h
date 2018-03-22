#pragma once
#include <iostream>
#include <fstream>
#include <Util\fileutil.h>
#include <Engine\Scene.h>
#include <Engine\Engine.h>
#include <Component\material.h>

namespace Engine
{
	class Project
	{
	public:
		Project(GLFWEngine* enginePointer);
		~Project() {};
		bool SetUpProjectDirectories();
		void AddModel();

		void Update();
		void Save();
	private:
		std::string m_Directory;
		bool CreateConfigFile(std::string path);

		Scene* m_Scene;
		GLFWEngine* m_EnginePointer;

		//NanoGUI Test
		nanogui::FormHelper *gui;
		nanogui::ref<nanogui::Window> windowGUI;
		bool m_bOrbit = false;
	};
}