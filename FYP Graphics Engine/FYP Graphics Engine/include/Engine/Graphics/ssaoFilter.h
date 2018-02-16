#pragma once
#pragma once
#include <Engine\Graphics\frameBuffer.h>
namespace Engine { namespace graphics {
	
	class SSAOFilter : public FrameBuffer
	{
	public:
		SSAOFilter(Window* window) : FrameBuffer(window, 1, new graphics::Shader("../Assets/Shaders/SSAO.vert", "../Assets/Shaders/SSAO.frag"))
		{

		};
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
