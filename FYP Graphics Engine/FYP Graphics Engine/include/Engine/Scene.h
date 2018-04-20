#pragma once
#include <Engine\Engine.h>
#include "gameObject.h"
#include <Component\modelRenderer.h>
#include <Component\texture.h>
#include <Component\material.h>
#include <Component\transform.h>
#include <Engine\Graphics\shader.h>
#include "Graphics\SkyBox.h"
#include <Engine\Graphics\postProcessingStack.h>
#include <Engine\UI\TransformWindow.h>
#include <Engine\UI\MaterialWindow.h>
#include <Util\fileutil.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\transform.hpp>
#include <vector>
#include <glad\glad.h>
#include <random>
#include <fstream>
#include <sstream>
namespace Engine
{
	//! Scene 
	/*!
	This class deals setting up, storing and drawing all objects for inital forward rendering.
	Contained in the Engine namespace.
	*/
	class Scene
	{
	public:
		//! Scene Contructor
		/*!
		Calls fucntion for inital set up of a scene
		\param enginePointer Referance to the GLFW engine
		\param load Boolea: True if the scene is being loaded from a save file
		*/
		Scene(GLFWEngine* enginePointer, bool load);
		//! Scene Destructor.
		/*!
		Cleans up memory in the class
		*/
		~Scene();
		//! The Update member function
		/*!
		Handles Camera movement using Arrow Keys and Left Control/Shift
		*/
		void Update(bool orbit);
		//! The Render member function
		/*!
		Binds defered renderer then draws objects including passing relevent infomation to shaders
		*/
		void Render();
		//! The AddObject member function
		/*!
		Used to add objects to the scene
		\param obj Pointer to new object
		*/
		void AddObject(GameObject* obj);
		//! The Save member function
		/*!
		Writes scene data to a text file
		\param savePath Path to save file
		*/
		void Save(std::string savePath);
		//! The Load member function
		/*!
		Loads scene data from a text file
		\param loadPath Path to save file
		*/
		void Load(std::string loadPath);

		//! The DeleteObject member function
		/*!
		Deletes the selected game object
		*/
		void DeleteObject();

	private:
		//! Private Shader pointer.
		/*! Pointer to the default shader for defered rendering*/
		graphics::Shader* m_DefaultShader = new graphics::Shader("../Assets/Shaders/differed.vert", "../Assets/Shaders/differed.frag");
		//! Private Shader pointer.
		/*! Pointer to the shader for rendering block colour lights*/
		graphics::Shader* m_LightShader = new graphics::Shader("../Assets/Shaders/light.vert", "../Assets/Shaders/light.frag");
		//! Private SkyBox pointer.
		/*! Pointer to the Skybox class, used for enviroment mapping */
		graphics::SkyBox* m_SkyBox;// = new graphics::SkyBox("../Assets/Textures/Cubemap_2/", "");
		//! Private GameObject pointer Vector.
		/*! Stores all the GameOBjects in the scene*/
		std::vector<GameObject*> v_Objects;
		//! Private GameObject pointer.
		/*! Pointer to the selected object*/
		GameObject* m_SelectedObject;
		//! Private GameObject pointer.
		/*! Pointer to the GameObject class, used for storing the Light object for debug drawing*/
		GameObject* m_LightObject;

		//! Private glm::mat4.
		/*! Perspective Matrix*/
		glm::mat4 P;
		//! Private glm::mat4.
		/*! View Matrix*/
		glm::mat4 V = glm::lookAt(glm::vec3(0, 7, -20), glm::vec3(0, 7, 0), glm::vec3(0, 1, 0));

		//! Private GLFWEngine pointer.
		/*! Pointer to the GLFWEngine class, used to access Engine variables and functions*/
		GLFWEngine* m_EnginePointer;
		//! Private PostProcessingStack pointer.
		/*! Pointer to the PostProcessingStack class, used to do all defered rendering and screen side effects*/
		graphics::PostProcessingStack* m_PostProcessing;


		//! Private TransformWindow pointer.
		/*! Pointer to the TransformWindow class, UI controls for transforms */
		UI::TransformWindow* m_TransformWindow;
		//! Private MaterialWindow pointer.
		/*! Pointer to the TransformWindow class, UI controls for materials */
		UI::MaterialWindow* m_MaterialWindow;
		//! Private float.
		/*! Current rotatation of camera around center */
		float theta = 0;
		//! Private float.
		/*! Rotation step */
		float step = 0.005f;
		//! Private float.
		/*! Distance from center in radians*/
		float radius = 15.f;
		//! Private float.
		/*! X axis rotation */
		float x = 0;
		//! Private float.
		/*! Z axis rotation */
		float y = 0;
		//! Private float.
		/*! Height of camera */
		float fYAxis = 0;

		//! Private float.
		/*! Rotation step in degrees */
		float rotationStep = step * (180 / 3.141f);
		//! Private glm::vec3.
		/*! Camera Position */
		glm::vec3 camPos = glm::vec3(15, 6, 0);

		

	};
}