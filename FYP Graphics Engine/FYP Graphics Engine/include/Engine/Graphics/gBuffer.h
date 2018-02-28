#pragma once
#include <Engine\Graphics\frameBuffer.h>

namespace Engine { namespace graphics {

	class GBuffer : public FrameBuffer
	{
	public:
		GBuffer(Window* window, int samples, Shader* shader);
		~GBuffer();
		GLuint GetNormal() { return m_NormalTexture; }
		GLuint GetPos() { return m_PositionTexture; }

		void Render();
	private:
		void createBuffer(GLenum texUnit, GLenum format, GLuint& texid);
		void createBufferMultisample(GLenum texUnit, GLenum format, GLuint& texid);
		Window* windowPointer;


	};

}	}