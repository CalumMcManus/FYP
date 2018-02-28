#include <Engine\Graphics\gBuffer.h>

Engine::graphics::GBuffer::GBuffer(Window * window, int samples, Shader * shader)
{
	CleanUp();
	m_Samples = samples;
	windowPointer = window;
	m_ScreenShader = shader;
	if (samples > 1)
	{
		glEnable(GL_MULTISAMPLE);
		//Set up FBO
		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		//Depth Buffer
		glGenRenderbuffers(1, &m_RBODepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBODepthBuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Samples, GL_DEPTH_COMPONENT, window->getWidth(), window->getHeight());

		createBufferMultisample(GL_TEXTURE1, GL_RGB16F, m_PositionTexture); //Position
		createBufferMultisample(GL_TEXTURE2, GL_RGB16F, m_NormalTexture); //Normal
		createBufferMultisample(GL_TEXTURE3, GL_RGB, m_Texture); //Color

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_PositionTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, m_NormalTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D_MULTISAMPLE, m_Texture, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBODepthBuffer);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_PositionTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_NormalTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_Texture, 0);

		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, drawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Frame Buffer: Contructor: Issue completing frame buffer with code " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	}
	else
	{
		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		//Depth Buffer
		glGenRenderbuffers(1, &m_RBODepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBODepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowPointer->getWidth(), windowPointer->getHeight());

		createBuffer(GL_TEXTURE1, GL_RGB16F, m_PositionTexture); //Position
		createBuffer(GL_TEXTURE2, GL_RGB16F, m_NormalTexture); //Normal
		createBuffer(GL_TEXTURE3, GL_RGB, m_Texture); //Color


		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBODepthBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PositionTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_Texture, 0);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_PositionTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_NormalTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_Texture, 0);

		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, drawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Frame Buffer: Contructor: Issue completing frame buffer with code " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	}
}

Engine::graphics::GBuffer::~GBuffer()
{
	CleanUp();
}

void Engine::graphics::GBuffer::createBuffer(GLenum texUnit, GLenum format, GLuint & texid)
{
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(
		GL_TEXTURE_2D, 0, format, windowPointer->getWidth(), windowPointer->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 0
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Engine::graphics::GBuffer::createBufferMultisample(GLenum texUnit, GLenum format, GLuint & texid)
{

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texid);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Samples, format, windowPointer->getWidth(), windowPointer->getHeight(), GL_TRUE);

}

void Engine::graphics::GBuffer::Render()
{
	//Unbind();
	glBindVertexArray(m_QuadVAO);
	glDisable(GL_DEPTH_TEST);
	m_ScreenShader->enable();
	//GLint baseImageLoc = glGetUniformLocation(m_ScreenShader->getID(), "texFramebuffer");
	//glUniform1i(baseImageLoc, 2);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, m_Texture);


	glDrawArrays(GL_TRIANGLES, 0, 6);
}
