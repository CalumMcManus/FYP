#pragma once
#pragma once
#include <Engine\Graphics\frameBuffer.h>
namespace Engine { namespace graphics {
	//! SSAOFilter 
	/*!
	SSAOFilter used for creating Screen Space Ambient Occlusion effect.
	Inherits from FrameBuffer.
	Contained in the Engine::graphics namespace.
	*/
	class SSAOFilter : public FrameBuffer
	{
	public:
		//! SSAOFilter Contructor
		/*!
		Passed the window pointer and SSAO shader to the FrameBuffer contructor
		\param window Window pointer
		*/
		SSAOFilter(Window* window) : FrameBuffer(window, new graphics::Shader("../Assets/Shaders/SSAO.vert", "../Assets/Shaders/SSAO.frag"))
		{

		};
		//! The Render member function
		/*!
		Overrides Framebuffer render function.
		Binds Position, normal and colour textures and draws SSAO texture to a screen quad
		*/
		void Render()
		{
			glBindVertexArray(m_QuadVAO);
			glDisable(GL_DEPTH_TEST);
			m_ScreenShader->enable();
			//Bind frameBufferTexture
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_PositionTexture);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_NormalTexture);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, m_Texture);


			glDrawArrays(GL_TRIANGLES, 0, 6);
		};
	private:
	};

} }
