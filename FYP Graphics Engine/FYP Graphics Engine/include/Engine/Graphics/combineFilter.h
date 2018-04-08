#pragma once
#include <Engine\Graphics\frameBuffer.h>
namespace Engine { namespace graphics {
	//! CombineFilter 
	/*!
	CombineFilter used for merging two frame buffer textures together
	Inherits from FrameBuffer.
	Contained in the Engine::graphics namespace.
	*/
	class CombineFilter : public FrameBuffer
	{
	public:
		//! CombineFilter Contructor
		/*!
		Passed the window pointer and Combine shader to the FrameBuffer contructor
		\param window Window pointer
		*/
		CombineFilter(Window* window) : FrameBuffer(window, new graphics::Shader("../Assets/Shaders/CombineTexture.vert", "../Assets/Shaders/CombineTexture.frag"))
		{

		};
		//! The Render member function
		/*!
		Overrides Framebuffer render function.
		Binds two textures and draws a combined texture to a screen quad
		*/
		void Render(GLuint textureTwo)
		{
			glBindVertexArray(m_QuadVAO);
			glDisable(GL_DEPTH_TEST);
			m_ScreenShader->enable();
			//Bind frameBufferTexture
			GLint baseImageLoc = glGetUniformLocation(m_ScreenShader->getID(), "texOne");
			glUniform1i(baseImageLoc, 2);		
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_Texture);
			//Bind second texture
			GLint texTwo = glGetUniformLocation(m_ScreenShader->getID(), "texTwo");
			glUniform1i(texTwo, 3);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, textureTwo);


			glDrawArrays(GL_TRIANGLES, 0, 6);
		};
	};
} }