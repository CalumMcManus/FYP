#pragma once
#include <Engine\Graphics\frameBuffer.h>
namespace Engine { namespace graphics {
	class CombineFilter : public FrameBuffer
	{
	public:
		CombineFilter(Window* window) : FrameBuffer(window, 1, new graphics::Shader("../Assets/Shaders/CombineTexture.vert", "../Assets/Shaders/CombineTexture.frag"))
		{

		};
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