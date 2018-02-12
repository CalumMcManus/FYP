#pragma once
#include <Engine\Graphics\frameBuffer.h>
#include <Engine\Graphics\combineFilter.h>
#include <Engine\Engine.h>
#include <nanogui\nanogui.h>
namespace Engine { namespace graphics {

	class PostProcessingStack
	{
	public:
		PostProcessingStack(GLFWEngine* enginePointer);
		~PostProcessingStack();
		void Bind();
		void Render();
	private:
		//Multi-Sample
		graphics::FrameBuffer* m_AABuffer;
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

		GLFWEngine* m_EnginePointer;

		nanogui::Window* m_PostProWindow;
		
		void SetUpUI();

		//Values
		unsigned int m_iBloomKernalSize = 1;
		float m_fBloomSigma = 1;
		float m_fBloomIntensity = 1;

		float m_fVignetteRadius = 0.75f;
		float m_fVignetteSoftness = 0.5f;


	};

} }