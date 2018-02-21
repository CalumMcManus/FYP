#pragma once
#include <Engine\Engine.h>
#include "gameObject.h"
#include <Component\modelRenderer.h>
#include <Component\texture.h>
#include <Component\transform.h>
#include <Engine\Graphics\shader.h>
#include "Graphics\SkyBox.h"
#include <Engine\Graphics\postProcessingStack.h>
#include <Engine\UI\TransformWindow.h>

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

	private:
		graphics::Shader* m_DefaultShader = new graphics::Shader("../Assets/Shaders/differed.vert", "../Assets/Shaders/differed.frag");
		graphics::SkyBox* m_SkyBox = new graphics::SkyBox("../Assets/Textures/Cubemap/", "");
		std::vector<GameObject*> v_Objects;

		//TODO:: Add to a camera class
		glm::mat4 P = glm::perspective(1.16937f, (float)1280 / (float)720, 0.1f, 150.f);//1.16937 = 67 degrees
		glm::mat4 V = glm::lookAt(glm::vec3(0, 7, -20), glm::vec3(0, 7, 0), glm::vec3(0, 1, 0));

		GLFWEngine* m_EnginePointer;
		graphics::PostProcessingStack* m_PostProcessing;


		//UI Stuff
		UI::TransformWindow* m_TransformWindow;
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

		//NanoGUI

		nanogui::Color m_SceneAmbient = nanogui::Color(1, 1, 1, 1);
		float m_fAmbientInten = 0;

		//Window Test
		nanogui::Window* m_SceneLighting;

		//gBuffer Test
		int m_iSamples = 16;

		GLuint m_DeferredFBOSingleSample;
		GLuint m_DepthBufferSS;
		GLuint m_ColorSS;
		GLuint m_NormalSS;
		GLuint m_PositionSS;
		GLuint m_PositionSSTex;

		GLuint m_DeferredFBO;
		GLuint m_DepthBuffer;
		GLuint m_Color;
		GLuint m_Normal;
		GLuint m_Position;
		GLuint m_PositionTex;
		void SetUpGBuffer();
		void createBuffer(GLenum texUnit, GLenum format, GLuint& texid);
		void createBufferMultisample(GLenum texUnit, GLenum format, GLuint& texid);
		graphics::Shader* m_ScreenShader = new graphics::Shader("../Assets/Shaders/AddSSAO.vert", "../Assets/Shaders/AddSSAO.frag");;

		GLuint m_QuadVAO;
		GLuint m_QuadVBO;

		float points[24] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f,  1.0f,  1.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f
		};

		//SSAO test
		GLuint m_SSAOFBO;
		GLuint m_SSAOTexture;
		GLuint m_NoiseTexture;
		graphics::Shader* m_SSAOShader = new graphics::Shader("../Assets/Shaders/SSAO.vert", "../Assets/Shaders/SSAO.frag");
		std::vector<glm::vec3> ssaoKernel;
	};
}