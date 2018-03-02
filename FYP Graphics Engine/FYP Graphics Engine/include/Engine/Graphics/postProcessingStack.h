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
namespace Engine { namespace graphics {

	class PostProcessingStack
	{
	public:
		PostProcessingStack(GLFWEngine* enginePointer);
		~PostProcessingStack();
		void Bind();
		void Render(glm::mat4 P, glm::mat4 View, glm::vec3 camPos);


	private:
		void SetUp();
		bool m_MS = false;
		bool m_SSAO = false;
		//Multi-Sample
		graphics::GBuffer* m_MSBuffer;
		//Multi-Sample
		graphics::GBuffer* m_SSBuffer;
		//No Filter
		graphics::FrameBuffer* m_FrameBuffer;
		//Grey Scale
		graphics::FrameBuffer* m_LumaBuffer;
		//Horizontal Blue
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

		graphics::SkyBox* m_SkyBox = new graphics::SkyBox("../Assets/Textures/Cubemap/", "");
		
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
	};

} }