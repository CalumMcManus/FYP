#pragma once
#include <iostream>
#include <fstream>
#include <Util\fileutil.h>
#include <Engine\Scene.h>
#include <Engine\Engine.h>
#include <Component\material.h>

namespace Engine
{
	//! Project 
	/*!
	This class deals setting up, saving and loading projects.
	Displays inital project UI.
	Contained in the Engine namespace.
	*/
	class Project
	{
	public:
		//! Project Contructor
		/*!
		Calls fucntion for inital set up of a project
		\param enginePointer Referance to the GLFW engine
		*/
		Project(GLFWEngine* enginePointer);
		//! Scene Destructor.
		/*!
		Cleans up memory in the class
		*/
		~Project() {};
		//! The SetUpProjectDirectories member function
		/*!
		Creates the project directories/folders and config file
		*/
		bool SetUpProjectDirectories();
		//! The AddModel member function
		/*!
		Creates and passed a new Model/Gameobject to the current scene
		*/
		void AddModel();
		//! The Render member function
		/*!
		Main project loop, calls scene to update and render
		*/
		void Update();
		//! The Save member function
		/*!
		Calls passes the project directory to the scene class to be used for saving
		*/
		void Save();
		//! The Load member function
		/*!
		Calls passes the project directory to the scene class to be used for loading
		*/
		void Load();
		//! The Init member function
		/*!
		Sets up the project and scene
		*/
		bool Init();
		//! The GetProjectDirectory member function
		/*!
		Returns the project directory as a string
		*/
		std::string GetProjectDirectory() { return m_Directory; };
	private:
		//! Private string.
		/*! Stores the project directory */
		std::string m_Directory = "./";

		//! The CreateConfigFile member function
		/*!
		Creates project config file at project directory
		\param path Project directory
		*/
		bool CreateConfigFile(std::string path);
		//! Private Scene pointer.
		/*! Pointer to the current scene*/
		Scene* m_Scene;
		//! Private GLFWEngine pointer.
		/*! Pointer to the GLFWEngine class, used to access Engine variables and functions*/
		GLFWEngine* m_EnginePointer;

		//NanoGUI
		nanogui::FormHelper *gui;
		nanogui::ref<nanogui::Window> windowGUI;
		nanogui::FormHelper *projectgui;
		nanogui::ref<nanogui::Window> projectSetup;
		bool m_bOrbit = false;
		bool m_bLoad = false;
	};
}