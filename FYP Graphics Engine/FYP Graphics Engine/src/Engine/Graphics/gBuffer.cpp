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

		createBufferMultisample(GL_TEXTURE1, GL_RGB16F, m_PositionTexture); //Position
		createBufferMultisample(GL_TEXTURE2, GL_RGB16F, m_NormalTexture); //Normal
		createBufferMultisample(GL_TEXTURE3, GL_RGB8, m_Texture); //Color
		createBufferMultisample(GL_TEXTURE3, GL_RGB8, m_UnlitTexture); //Color
		createBufferMultisample(GL_TEXTURE4, GL_DEPTH_COMPONENT32F, m_DepthTexture); //Color

		

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_PositionTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, m_NormalTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D_MULTISAMPLE, m_Texture, 0);	
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D_MULTISAMPLE, m_UnlitTexture, 0);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_PositionTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_NormalTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_Texture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, m_UnlitTexture, 0);
		//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture, 0);

		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, drawBuffers);

		//Depth Buffer
		glGenRenderbuffers(1, &m_RBODepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBODepthBuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Samples, GL_DEPTH_COMPONENT32F, window->getWidth(), window->getHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBODepthBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_DepthTexture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Frame Buffer: Contructor: Issue completing frame buffer with code " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	}
	else
	{
		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		createBuffer(GL_TEXTURE1, GL_RGB16F, m_PositionTexture); //Position
		createBuffer(GL_TEXTURE2, GL_RGB16F, m_NormalTexture); //Normal
		createBuffer(GL_TEXTURE3, GL_RGB8, m_Texture); //Color
		createBuffer(GL_TEXTURE3, GL_RGB8, m_UnlitTexture); //Color
		glGenTextures(1, &m_DepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, windowPointer->getWidth(), windowPointer->getHeight(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PositionTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_Texture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_UnlitTexture, 0);
		

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_PositionTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_NormalTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_Texture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, m_UnlitTexture, 0);
		//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture, 0);

		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, drawBuffers);

		//Depth Buffer
		glGenRenderbuffers(1, &m_RBODepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBODepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, windowPointer->getWidth(), windowPointer->getHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBODepthBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


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
