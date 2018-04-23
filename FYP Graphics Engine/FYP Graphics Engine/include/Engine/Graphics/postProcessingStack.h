#pragma once
#include <Engine\Graphics\frameBuffer.h>
#include <Engine\Graphics\combineFilter.h>
#include <Engine\Graphics\shadowBuffer.h>
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
			/*! RGB Light Colour */
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
			//! Public bool variable.
			/*! True if the light should be delete from the scene */
			bool toDelete = false;

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
		void Render(glm::mat4 P, glm::mat4 View, glm::vec3 camPos, std::vector<GameObject*> objects);
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

		glm::vec3 getDirLight() { return dirRotation;  }
	private:
		//! Private vector of light pointers.
		/*! Contains all the lights in the scene */
		std::vector<Light*> m_Lights;
		//! Private light pointer.
		/*! Contains the currently selected light */
		Light *m_SelectedLight;
		//! The SetUp member function
		/*!
		Sets up all Frame Buffers including the GBuffer.
		Also used for cleanig up memory for recreating buffers.
		*/
		void SetUp();
		//! Private Bool variable.
		/*! Is true if Multisampling is enabled*/
		bool m_MS = false;
		//! Private Bool variable.
		/*! Is true if Ambient Occlution is enabled*/
		bool m_SSAO = false;
		//! Private Bool variable.
		/*! Is true if Outline is enabled*/
		bool m_Outline = false;

		//! Private GBuffer pointer.
		/*! Multisampled GBuffer*/
		graphics::GBuffer* m_MSBuffer;
		//! Private GBuffer pointer.
		/*! Single sample GBuffer */
		graphics::GBuffer* m_SSBuffer;
		//! Private FrameBuffer pointer.
		/*! No Filter FrameBuffer storing screen infomation for PostProcessing effect */
		graphics::FrameBuffer* m_FrameBuffer;
		//! Private FrameBuffer pointer.
		/*! Greyscale filter using Luma convertion */
		graphics::FrameBuffer* m_LumaBuffer;
		//! Private FrameBuffer pointer.
		/*! Horizontal Gaussian blur filter */
		graphics::FrameBuffer* m_HBlurBuffer;
		//! Private FrameBuffer pointer.
		/*! Vertical Gaussian blur filter */
		graphics::FrameBuffer* m_VBlurBuffer;
		//! Private FrameBuffer pointer.
		/*! Combines the base colour texture with bloom filter */
		graphics::CombineFilter* m_Bloom;
		//! Private FrameBuffer pointer.
		/*! Vignette Filter */
		graphics::FrameBuffer* m_Vignette;
		//! Private FrameBuffer pointer.
		/*! Vignette Filter */
		graphics::FrameBuffer* m_Reflection;
		//! Private ShadowBuffer pointer.
		/*! Shaow Map Filter */
		graphics::ShadowBuffer* m_ShadowMap;

		//! Private Shader pointer.
		/*! No Filter Shader */
		graphics::Shader* m_NoFilter = new graphics::Shader("../Assets/Shaders/NoFilter.vert", "../Assets/Shaders/NoFilter.frag");
		//! Private Shader pointer.
		/*! No Filter Shader */
		graphics::Shader* m_ShadowMapShader = new graphics::Shader("../Assets/Shaders/ShadowMap.vert", "../Assets/Shaders/ShadowMap.frag");
		//! Private Shader pointer.
		/*! Horizontal Gaussian blur Shader */
		graphics::Shader* m_BlurH = new graphics::Shader("../Assets/Shaders/Bloom.vert", "../Assets/Shaders/Bloom.frag");
		//! Private Shader pointer.
		/*! Vertical Gaussian blur Shader */
		graphics::Shader* m_BlurV = new graphics::Shader("../Assets/Shaders/Bloom2.vert", "../Assets/Shaders/Bloom2.frag");
		//! Private GLFWEngine pointer.
		/*! Pointer to the GLFWEngine class, used to access Engine variables and functions*/
		GLFWEngine* m_EnginePointer;
		//! Private SkyBox pointer.
		/*! Pointer to the Skybox class */
		graphics::SkyBox* m_SkyBox;
		//! The lerp member function
		/*!
		Linearly Interpolates between two numbers based on f
		\param a float First number
		\param b float Second number
		\param f float Interpolation float
		*/
		float lerp(float a, float b, float f)
		{
			return a + f * (b - a);
		}


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
		graphics::Shader* m_ReflectionShader = new graphics::Shader("../Assets/Shaders/Reflection.vert", "../Assets/Shaders/Reflection.frag");
		//No Filter
		graphics::FrameBuffer* m_FrameBufferAO;
		//No Filter
		graphics::FrameBuffer* m_FinalBlueAO;
		//Horizontal Blue
		graphics::FrameBuffer* m_HBlurBufferAO;
		//Vertical Blur
		graphics::FrameBuffer* m_VBlurBufferAO;


		void SetUpUI();

		nanogui::Window* m_PostProWindow;

		//Values
		int m_iSamples = 4;
		unsigned int m_iBloomKernalSize = 1;
		float m_fBloomSigma = 1;
		float m_fBloomIntensity = 1;

		float m_fVignetteRadius = 0.75f;
		float m_fVignetteSoftness = 0.5f;

		

		
		//LightingNanoGUI

		nanogui::Color m_SceneAmbient = nanogui::Color(255, 255, 255, 255);
		float m_fAmbientInten = 0.5f;
		nanogui::Window* m_SceneLighting;

		//Cel Shading
		bool m_bCellShading;
		int m_iCells;
		//Directional Light
		float m_fDirectionalIntencity;
		glm::vec3 m_DirectionalDir = glm::vec3(0.001f, -1, 0);
		glm::vec3 m_DirectionalColour = glm::vec3(1, 1, 1);
		float m_DirectionalX;
		float m_DirectionalY;
		glm::vec3 dirRotation;
	};

} }