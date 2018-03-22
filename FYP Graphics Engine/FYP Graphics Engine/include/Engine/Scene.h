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

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\transform.hpp>
#include <vector>
#include <glad\glad.h>
#include <random>
namespace Engine
{
	class Scene
	{
	public:
		Scene(GLFWEngine* enginePointer);
		~Scene();

		void Update(bool orbit);
		void Render();

		void AddObject(GameObject* obj);

		void Save(std::string savePath);

	private:
		graphics::Shader* m_DefaultShader = new graphics::Shader("../Assets/Shaders/differed.vert", "../Assets/Shaders/differed.frag");
		graphics::Shader* m_LightShader = new graphics::Shader("../Assets/Shaders/light.vert", "../Assets/Shaders/light.frag");
		graphics::SkyBox* m_SkyBox = new graphics::SkyBox("../Assets/Textures/Cubemap/", "");
		std::vector<GameObject*> v_Objects;

		GameObject* m_LightObject;

		//TODO:: Add to a camera class
		glm::mat4 P = glm::perspective(1.16937f, (float)1280 / (float)720, 0.1f, 150.f);//1.16937 = 67 degrees
		glm::mat4 V = glm::lookAt(glm::vec3(0, 7, -20), glm::vec3(0, 7, 0), glm::vec3(0, 1, 0));

		GLFWEngine* m_EnginePointer;
		graphics::PostProcessingStack* m_PostProcessing;


		//UI Stuff
		UI::TransformWindow* m_TransformWindow;
		UI::MaterialWindow* m_MaterialWindow;
		//TODO:: In Camera class create orbit functionality
		//Orbit Camera position with bounce
		float theta = 0;
		float step = 0.005f;
		float radius = 15.f;
		float x = 0;
		float y = 0;

		float rotationStep = step * (180 / 3.141f);

		bool yAxis = true;
		float fYAxis = 0;
		glm::vec3 camPos = glm::vec3(0, 6, -15);

		

	};
}