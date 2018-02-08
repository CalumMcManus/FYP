#include <Engine\Graphics\frameBuffer.h>

Engine::graphics::FrameBuffer::FrameBuffer(Window * window, int samples)
{
	m_Samples = samples;
	//Set up FBO
	glGenFramebuffers(1, &m_FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &m_Texture);
	if (samples > 1)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_Texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, 1280, 720, GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_Texture, 0);

		glGenRenderbuffers(1, &m_TexColorBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_TexColorBuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGB, 1280, 720);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_TexColorBuffer);

		glGenRenderbuffers(1, &m_RBODepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBODepthBuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, 1280, 720);
		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBODepthBuffer
		);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_Texture, 0);

	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, 0
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0
		);

		// Create color render buffer
		glGenRenderbuffers(1, &m_TexColorBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_TexColorBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, 1280, 720);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_TexColorBuffer);

		// Create depth render buffer (This is optional)
		glGenRenderbuffers(1, &m_RBODepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBODepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBODepthBuffer);

		// Bind Texture assuming we have created a texture
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_Texture, 0);
	}
	
	

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Frame Buffer: Contructor: Issue completing frame buffer with code " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << " Samples: "<< samples << std::endl;


	//Set Up Shader
	m_ScreenShader = new Shader("../Assets/Shaders/Bloom.vert", "../Assets/Shaders/Bloom.frag");

	//Setup Quad VAO
	
	glGenBuffers(1, &m_QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_QuadVAO);
	glBindVertexArray(m_QuadVAO);
	//glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLint posAttrib = glGetAttribLocation(m_ScreenShader->getID(), "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	GLint texAttrib = glGetAttribLocation(m_ScreenShader->getID(), "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

void Engine::graphics::FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}

void Engine::graphics::FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::graphics::FrameBuffer::Render()
{
	Unbind();
	glBindVertexArray(m_QuadVAO);
	glDisable(GL_DEPTH_TEST);
	m_ScreenShader->enable();
	m_ScreenShader->setUniform1f("time", glfwGetTime());
	GLint baseImageLoc = glGetUniformLocation(m_ScreenShader->getID(), "texFramebuffer");
	glUniform1i(baseImageLoc, 2);
	glActiveTexture(GL_TEXTURE2);
	if(m_Samples > 1)
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_Texture);
		else
			glBindTexture(GL_TEXTURE_2D, m_Texture);
	

	glDrawArrays(GL_TRIANGLES, 0, 6);
}
