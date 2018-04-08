#pragma once
#include <Engine\Graphics\frameBuffer.h>
#include <Engine\Graphics\combineFilter.h>
#include <Engine\Graphics\gBuffer.h>
#include <Engine\Engine.h>
#include <gtc\type_ptr.hpp>
#include <random>
#include <nanogui\nanogui.h>

#include <Engine\Graphics\SkyBox.h>
#include <gtx\transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx\quaternion.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtx/euler_angles.hpp>
#include <glm.hpp>
#include <fstream>
namespace Engine { namespace graphics {
	//! PostProcessingStack 
	/*!
	Handles all defered rendering and screen effects using Frame Buffers and the GBuffer
	Contained in the Engine::UI namespace.
	*/
	class PostProcessingStack
	{
	
	public:
		//! Light 
		/*!
		Light Struct containing all light related data used for creating Point and Spot lights
		Contained in the Engine::UI:PostProcessingStack namespace.
		*/
		struct Light
		{
			//! Public glm::vec3 variable.
			/*! Light Position */
			glm::vec3 Pos;
			//! Public glm::vec3 variable.
			/*! Light Colour */
			glm::vec3 Color;
			//! Public glm::vec3 variable.
			/*! Light Rotation */
			glm::vec3 Rot;
			//! Public float variable.
			/*! Light Cone angle (spot light) */
			float Angle;
			//! Public float variable.
			/*! Light Radius (spot light) */
			float Radius;
			//! Public float variable.
			/*! Light Intencity */
			float Intencity;
			//! Light Contructor
			/*!
			Empty contructor
			*/
			Light() {};
			//! Light Contructor
			/*!
			Sets position, colour, radius and intencity of the light
			\param pos glm::vec3 light position
			\param color glm::vec3 light colour
			\param radius float light radius
			\param inten float light intencity
			*/
			Light(glm::vec3 pos, glm::vec3 color, float radius, float inten)
			{
				Pos = pos;
				Rot = glm::vec3(0, 0, 0);
				Angle = 180; //Point Light
				Color = color;
				Radius = radius;
				Intencity = inten;
			}

		};
		//! Light Contructor
		/*!
		Sets up SSAO settings and quad for rendering.
		Calls function to create Post Processing and Lighting UI
		\param enginePointer GLFWEngine pointer
		\param skyBox Skybox pointer
		\param load Bool true if the current scene is loaded from a text file
		*/
		PostProcessingStack(GLFWEngine* enginePointer, SkyBox* skyBox, bool load);
		//! PostProcessingStack Destructor.
		/*!
		Cleans up memory in the class
		*/
		~PostProcessingStack();
		//! The Bind member function
		/*!
		Binds GBuffer
		*/
		void Bind();
		//! The Render member function
		/*!
		Renders all defered rendering stages while passing infomation to required shaders
		*/
		void Render(glm::mat4 P, glm::mat4 View, glm::vec3 camPos);
		//! The Lights member function
		/*!
		Returns a vector of pointers to all lights in the scene
		*/
		std::vector<Light*> Lights() { return m_Lights; };
		//! The SetSelectedLight member function
		/*!
		Passed in the selected light so that the colour can be changed in the Lighting UI
		\param light Light pointer of selected light
		*/
		void SetSelectedLight(Light* light) { m_SelectedLight = light; }
		//! The Save member function
		/*!
		Saves Lighting and Post processing settings to the save text file
		\param file ofstream File stream to open save file
		*/
		void Save(std::ofstream& file);
		//! The Load member function
		/*!
		Load Lighting and Post processing settings from the save text file
		\param file ofstream File stream to open save file
		*/
		void Load(std::ifstream& file);
	private:
		std::vector<Light*> m_Lights;
		Light *m_SelectedLight;
		void SetUp();
		bool m_MS = false;
		bool m_SSAO = false;
		bool m_Outline = false;
		//Multi-Sample
		graphics::GBuffer* m_MSBuffer;
		//Multi-Sample
		graphics::GBuffer* m_SSBuffer;
		//No Filter
		graphics::FrameBuffer* m_FrameBuffer;
		//Grey Scale
		graphics::FrameBuffer* m_LumaBuffer;
		//Horizontal Blur
		graphics::FrameBuffer* m_HBlurBuffer;
		//Vertical Blur
		graphics::FrameBuffer* m_VBlurBuffer;
		//Combine blurs and no filter to make bloom
		graphics::CombineFilter* m_Bloom;
		//Vignette filter
		graphics::FrameBuffer* m_Vignette;

		graphics::Shader* m_NoFilter = new graphics::Shader("../Assets/Shaders/NoFilter.vert", "../Assets/Shaders/NoFilter.frag");
		graphics::Shader* m_BlurH = new graphics::Shader("../Assets/Shaders/Bloom.vert", "../Assets/Shaders/Bloom.frag");
		graphics::Shader* m_BlurV = new graphics::Shader("../Assets/Shaders/Bloom2.vert", "../Assets/Shaders/Bloom2.frag");

		GLFWEngine* m_EnginePointer;

		nanogui::Window* m_PostProWindow;

		graphics::SkyBox* m_SkyBox;
		
		void SetUpUI();

		//Values
		int m_iSamples = 4;
		unsigned int m_iBloomKernalSize = 1;
		float m_fBloomSigma = 1;
		float m_fBloomIntensity = 1;

		float m_fVignetteRadius = 0.75f;
		float m_fVignetteSoftness = 0.5f;

		//SSAO test
	
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

		GLuint m_SSAOFBO;
		GLuint m_SSAOTexture;
		GLuint m_NoiseTexture;
		graphics::Shader* m_SSAOShader = new graphics::Shader("../Assets/Shaders/SSAO.vert", "../Assets/Shaders/SSAO.frag");
		std::vector<glm::vec3> ssaoKernel;
		graphics::Shader* m_AddSSAO = new graphics::Shader("../Assets/Shaders/AddSSAO.vert", "../Assets/Shaders/AddSSAO.frag");

		//No Filter
		graphics::FrameBuffer* m_FrameBufferAO;
		//No Filter
		graphics::FrameBuffer* m_FinalBlueAO;
		//Horizontal Blue
		graphics::FrameBuffer* m_HBlurBufferAO;
		//Vertical Blur
		graphics::FrameBuffer* m_VBlurBufferAO;

		float lerp(float a, float b, float f)
		{
			return a + f * (b - a);
		}

		//LightingNanoGUI

		nanogui::Color m_SceneAmbient = nanogui::Color(255, 255, 255, 255);
		float m_fAmbientInten = 0.5f;
		nanogui::Window* m_SceneLighting;
	};

} }