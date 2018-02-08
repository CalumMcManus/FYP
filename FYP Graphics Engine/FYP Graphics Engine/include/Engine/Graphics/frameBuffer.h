#pragma once
#include <glad\glad.h>
#include <Engine\window.h>
#include <Engine\Graphics\shader.h>
#include <iostream>
namespace Engine { namespace graphics {

	class FrameBuffer
	{
	public:
		FrameBuffer(Window* window, int samples);
		void Bind();
		void Unbind();
		void Render();
		GLuint GetBufferID() { return m_FrameBuffer; };
	private:
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

		GLuint m_FrameBuffer;
		GLuint m_TexColorBuffer;
		GLuint m_RBODepthBuffer;
		GLuint m_Texture;

		int m_Samples;

		Shader* m_ScreenShader;

	};


} }