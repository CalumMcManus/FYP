#include <Engine\Graphics\postProcessingStack.h>

Engine::graphics::PostProcessingStack::PostProcessingStack(GLFWEngine * enginePointer, SkyBox* skyBox, bool load)
{
	m_EnginePointer = enginePointer;
	m_SkyBox = skyBox;
	//m_Lights.push_back(new Light(glm::vec3(10, 0, 0), glm::vec3(1, 0, 1), 10.0f, 3.0f));
	//m_Lights.push_back(new Light(glm::vec3(-10, 0, 0), glm::vec3(0, 1, 1), 10.0f, 3.0f));
	//Setup Quad VAO

	glGenBuffers(1, &m_QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_QuadVAO);
	glBindVertexArray(m_QuadVAO);
	//glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLint posAttrib = glGetAttribLocation(m_AddSSAO->getID(), "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	GLint texAttrib = glGetAttribLocation(m_AddSSAO->getID(), "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	//SSAO
	std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
	std::default_random_engine generator;

	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = (float)i / 64.0f;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}

	glGenTextures(1, &m_NoiseTexture);
	glBindTexture(GL_TEXTURE_2D, m_NoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetUp();

	if(!load)
		SetUpUI();
}
void Engine::graphics::PostProcessingStack::Save(std::ofstream & file)
{
	file << "#Lighting" << std::endl;

	file << "AmbientColour: " + std::to_string(m_SceneAmbient.x()) + " " + std::to_string(m_SceneAmbient.y()) + " " + std::to_string(m_SceneAmbient.z()) << std::endl;
	file << "AmbientInten: " + std::to_string(m_fAmbientInten) << std::endl;
	for (unsigned int i = 0; i < m_Lights.size(); i++)
	{
		file << "#LBEGIN" << std::endl;
		file << "Pos: " + std::to_string(m_Lights[i]->Pos.x) + " " + std::to_string(m_Lights[i]->Pos.y) + " " + std::to_string(m_Lights[i]->Pos.z) << std::endl;
		file << "Rot: " + std::to_string(m_Lights[i]->Rot.x) + " " + std::to_string(m_Lights[i]->Rot.y) + " " + std::to_string(m_Lights[i]->Rot.z) << std::endl;
		file << "Color: " + std::to_string(m_Lights[i]->Color.x) + " " + std::to_string(m_Lights[i]->Color.y) + " " + std::to_string(m_Lights[i]->Color.z) << std::endl;
		file << "Inten: " + std::to_string(m_Lights[i]->Intencity) << std::endl;
		file << "Radius: " + std::to_string(m_Lights[i]->Radius) << std::endl;
		file << "Angle: " + std::to_string(m_Lights[i]->Angle) << std::endl;
		file << "#LEND " << std::endl;
	}

	file << "#Effects" << std::endl;
	file << "MSAA: " + std::to_string((int)m_MS) << std::endl;
	file << "SSAO: " + std::to_string((int)m_SSAO) << std::endl;
	file << "BloomInten: " + std::to_string(m_fBloomIntensity) << std::endl;
	file << "BloomKernal: " + std::to_string(m_iBloomKernalSize) << std::endl;
	file << "BloomSigma: " + std::to_string(m_fBloomSigma) << std::endl;
	file << "VignetteRadius: " + std::to_string(m_fVignetteRadius) << std::endl;
	file << "VignetteSoft: " + std::to_string(m_fVignetteSoftness) << std::endl;
	file << "Outline: " + std::to_string((int)m_Outline) << std::endl;
	file << "Cel Shading: " + std::to_string((int)m_bCellShading) << std::endl;
	file << "Cels: " + std::to_string(m_iCells) << std::endl;
	file << "DirInten: " + std::to_string(m_fDirectionalIntencity) << std::endl;
	file << "DirColour: " + std::to_string(m_DirectionalColour.x) + " " + std::to_string(m_DirectionalColour.y) + " " + std::to_string(m_DirectionalColour.z) << std::endl;

}
void Engine::graphics::PostProcessingStack::Load(std::ifstream & file)
{
	std::string line;
	std::istringstream iss;
	std::string s;
	float x, y, z;
	std::getline(file, line);
	iss = std::istringstream(line);
	iss >> s;
	iss >> x; iss >> y; iss >> z;
	m_SceneAmbient = nanogui::Color(x, y, z, 1.0f);
	std::getline(file, line);
	iss = std::istringstream(line);
	iss >> s;
	iss >> m_fAmbientInten;
	while (!file.eof())
	{
		std::getline(file, line);
		iss = std::istringstream(line);
		iss >> s;
		if (s == "#LBEGIN")
		{
			Light* light = new Light();
			std::cout << "light" << std::endl;

			std::getline(file, line);
			iss = std::istringstream(line);
			
			glm::vec3 tempVector;
			iss >> s;
			iss >> tempVector.x; iss >> tempVector.y; iss >> tempVector.z;
			light->Pos = tempVector;
			tempVector = glm::vec3();
			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> tempVector.x; iss >> tempVector.y; iss >> tempVector.z;
			light->Rot = tempVector;
			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> tempVector.x;
			iss >> tempVector.y;
			iss >> tempVector.z;
			light->Color = tempVector;
			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> light->Intencity;
			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> light->Radius;
			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> light->Angle;
			m_Lights.push_back(light);
		}

		if (s == "#Effects")
		{
			int boolean;
			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> boolean;
			m_MS = boolean;
			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> boolean;
			m_SSAO = boolean;

			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> m_fBloomIntensity;

			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> m_iBloomKernalSize;

			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> m_fBloomSigma;

			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> m_fVignetteRadius;

			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> m_fVignetteSoftness;

			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> boolean;
			m_Outline = boolean;

			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> boolean;
			m_bCellShading = boolean;

			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> m_iCells;

			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> m_fDirectionalIntencity;

			std::getline(file, line);
			iss = std::istringstream(line);
			iss >> s;
			iss >> x; iss >> y; iss >> z;
			m_DirectionalColour = glm::vec3(x, y, z);
		}
	}
	SetUpUI();
	
}
void Engine::graphics::PostProcessingStack::SetUp()
{
	delete m_MSBuffer; m_MSBuffer = nullptr;
	delete m_SSBuffer; m_SSBuffer = nullptr;
	delete m_FrameBuffer; m_FrameBuffer = nullptr;
	delete m_LumaBuffer; m_LumaBuffer = nullptr;
	delete m_HBlurBuffer; m_HBlurBuffer = nullptr;
	delete m_VBlurBuffer; m_VBlurBuffer = nullptr;
	delete m_Vignette; m_Vignette = nullptr;
	delete m_FrameBufferAO; m_FrameBufferAO = nullptr;
	delete m_HBlurBufferAO; m_HBlurBufferAO = nullptr;
	delete m_VBlurBufferAO; m_VBlurBufferAO = nullptr;
	delete m_FinalBlueAO; m_FinalBlueAO = nullptr;
	delete m_Bloom; m_Bloom = nullptr;
	delete m_Reflection; m_Reflection = nullptr;

	m_MSBuffer = new graphics::GBuffer(m_EnginePointer->m_Window, m_iSamples, m_NoFilter);
	m_SSBuffer = new graphics::GBuffer(m_EnginePointer->m_Window, 1, m_NoFilter);

	m_FrameBuffer = new graphics::FrameBuffer(m_EnginePointer->m_Window, m_NoFilter);
	m_LumaBuffer = new graphics::FrameBuffer(m_EnginePointer->m_Window, new graphics::Shader("../Assets/Shaders/BrightOnly.vert", "../Assets/Shaders/BrightOnly.frag"));
	m_HBlurBuffer = new graphics::FrameBuffer(m_EnginePointer->m_Window, m_BlurH);
	m_VBlurBuffer = new graphics::FrameBuffer(m_EnginePointer->m_Window, m_BlurV);
	m_Vignette = new graphics::FrameBuffer(m_EnginePointer->m_Window, new graphics::Shader("../Assets/Shaders/Vignette.vert", "../Assets/Shaders/Vignette.frag"));

	m_FrameBufferAO = new graphics::FrameBuffer(m_EnginePointer->m_Window, m_NoFilter);
	m_HBlurBufferAO = new graphics::FrameBuffer(m_EnginePointer->m_Window, m_BlurH);
	m_VBlurBufferAO = new graphics::FrameBuffer(m_EnginePointer->m_Window, m_BlurV);
	m_FinalBlueAO = new graphics::FrameBuffer(m_EnginePointer->m_Window, m_NoFilter);
	m_Bloom = new graphics::CombineFilter(m_EnginePointer->m_Window);

	m_Reflection = new graphics::FrameBuffer(m_EnginePointer->m_Window, m_NoFilter);
	
	glDeleteFramebuffers(1, &m_SSAOFBO);
	glDeleteTextures(1, &m_SSAOTexture);

	glGenFramebuffers(1, &m_SSAOFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOFBO);

	glGenTextures(1, &m_SSAOTexture);
	glBindTexture(GL_TEXTURE_2D, m_SSAOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSAOTexture, 0);
}
Engine::graphics::PostProcessingStack::~PostProcessingStack()
{
}

void Engine::graphics::PostProcessingStack::Bind()
{
	if (m_MS)
	{
		m_MSBuffer->Bind();
	}
	else
		m_SSBuffer->Bind();
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawBuffers);
	m_EnginePointer->m_Window->Clear();
}

void Engine::graphics::PostProcessingStack::Render(glm::mat4 P, glm::mat4 View, glm::vec3 camPos)
{
	if (m_MS)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSBuffer->GetBufferID());
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_SSBuffer->GetBufferID());
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSBuffer->GetBufferID());
		glReadBuffer(GL_COLOR_ATTACHMENT3);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_SSBuffer->GetBufferID());
		glDrawBuffer(GL_COLOR_ATTACHMENT3);
		glBlitFramebuffer(0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSBuffer->GetBufferID());
		glReadBuffer(GL_COLOR_ATTACHMENT2);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_SSBuffer->GetBufferID());
		glDrawBuffer(GL_COLOR_ATTACHMENT2);
		glBlitFramebuffer(0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSBuffer->GetBufferID());
		glReadBuffer(GL_COLOR_ATTACHMENT1);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_SSBuffer->GetBufferID());
		glDrawBuffer(GL_COLOR_ATTACHMENT1);
		glBlitFramebuffer(0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);


		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSBuffer->GetBufferID());
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_SSBuffer->GetBufferID());
		glDrawBuffer(GL_NONE);
		glBlitFramebuffer(0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			GL_DEPTH_BUFFER_BIT,
			GL_NEAREST);
	}


	glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOFBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT);
	m_SSAOShader->enable();
	GLint posImageLoc = glGetUniformLocation(m_SSAOShader->getID(), "gPosition");
	glUniform1i(posImageLoc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetPos());
	GLint normImageLoc = glGetUniformLocation(m_SSAOShader->getID(), "gNormal");
	glUniform1i(normImageLoc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetNormal());
	GLint noiseImageLoc = glGetUniformLocation(m_SSAOShader->getID(), "texNoise");
	glUniform1i(noiseImageLoc, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_NoiseTexture);

	glUniform3fv(glGetUniformLocation(m_SSAOShader->getID(), "samples"), 64,
		glm::value_ptr(ssaoKernel[0]));
	m_SSAOShader->setUniformMat4("projection", P);
	m_SSAOShader->setUniform2f("noiseScale", glm::vec2(m_EnginePointer->m_Window->getWidth()/4, m_EnginePointer->m_Window->getHeight()/4));

    

	m_FrameBufferAO->Bind();
	glBindVertexArray(m_QuadVAO);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	m_HBlurBufferAO->Bind();
	m_HBlurBufferAO->GetShader()->enable();
	m_HBlurBufferAO->GetShader()->setUniform1f("sigmaValue", 2);
	m_HBlurBufferAO->GetShader()->setUniform1i("kernalSize", 25);
	m_HBlurBufferAO->GetShader()->setUniform2f("resolution", glm::vec2(m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight()));
	m_EnginePointer->m_Window->Clear();
	m_FrameBufferAO->Render();
	
	m_VBlurBufferAO->Bind();
	m_VBlurBufferAO->GetShader()->enable();
	m_VBlurBufferAO->GetShader()->setUniform1f("sigmaValue", 2);
	m_VBlurBufferAO->GetShader()->setUniform1i("kernalSize", 25);	
	m_VBlurBufferAO->GetShader()->setUniform2f("resolution", glm::vec2(m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight()));
	m_EnginePointer->m_Window->Clear();
	m_HBlurBufferAO->Render();
	
	m_FinalBlueAO->Bind();
	m_EnginePointer->m_Window->Clear();
	m_VBlurBufferAO->Render();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_EnginePointer->m_Window->Clear();
	m_FinalBlueAO->Render();
	
	m_EnginePointer->m_Window->Clear();
		
	//Lighting Pass

	m_FrameBuffer->Bind();
	glBindVertexArray(m_QuadVAO);
	glDisable(GL_DEPTH_TEST);

	m_AddSSAO->enable();
	
	if (m_MS)
	{
		GLint baseImageLoc = glGetUniformLocation(m_AddSSAO->getID(), "gAlbedo");
		glUniform1i(baseImageLoc, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MSBuffer->GetTextureID());

		GLint posLoc = glGetUniformLocation(m_AddSSAO->getID(), "gPosition");
		glUniform1i(posLoc, 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MSBuffer->GetPos());

		GLint normLoc = glGetUniformLocation(m_AddSSAO->getID(), "gNormal");
		glUniform1i(normLoc, 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MSBuffer->GetNormal());

		GLint ssaoLoc = glGetUniformLocation(m_AddSSAO->getID(), "ssaoTex");
		glUniform1i(ssaoLoc, 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_FinalBlueAO->GetTextureID());

		GLint depthLoc = glGetUniformLocation(m_AddSSAO->getID(), "gDepth");
		glUniform1i(depthLoc, 6);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MSBuffer->GetDepth());

		GLint compLoc = glGetUniformLocation(m_AddSSAO->getID(), "gComponent");
		glUniform1i(compLoc, 7);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MSBuffer->GetAdditional());
		m_AddSSAO->setUniform1i("Samples", m_iSamples);
	}
	else
	{
		GLint baseImageLoc = glGetUniformLocation(m_AddSSAO->getID(), "gAlbedoSS");
		glUniform1i(baseImageLoc, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetTextureID());
		//
		GLint ssaoLoc = glGetUniformLocation(m_AddSSAO->getID(), "ssaoTex");
		glUniform1i(ssaoLoc, 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_FinalBlueAO->GetTextureID());
		//
		GLint posLoc = glGetUniformLocation(m_AddSSAO->getID(), "gPositionSS");
		glUniform1i(posLoc, 8);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetPos());
		//
		GLint normLoc = glGetUniformLocation(m_AddSSAO->getID(), "gNormalSS");
		glUniform1i(normLoc, 9);
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetNormal());
		//
		GLint depthLoc = glGetUniformLocation(m_AddSSAO->getID(), "gDepthSS");
		glUniform1i(depthLoc, 10);
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetDepth());
		//
		GLint compLoc = glGetUniformLocation(m_AddSSAO->getID(), "gComponentSS");
		glUniform1i(compLoc, 11);
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetAdditional());
		
		m_AddSSAO->setUniform1i("Samples", 1);
	}
	

	
	
	glm::vec3 dirRotation =
		m_DirectionalDir
		* glm::toQuat(glm::rotate(glm::radians(m_DirectionalX*360.0f), glm::vec3(1, 0, 0)));
	
	dirRotation = dirRotation * glm::toQuat(glm::rotate(glm::radians(m_DirectionalY*360.0f), glm::vec3(0, 1, 0)));

	m_AddSSAO->setUniform1i("SSAO", m_SSAO);
	m_AddSSAO->setUniform1i("Outline", m_Outline);
	m_AddSSAO->setUniform1i("CellShading", m_bCellShading);
	m_AddSSAO->setUniform1i("Cells", m_iCells);
	m_AddSSAO->setUniform1f("DirInten", m_fDirectionalIntencity);
	m_AddSSAO->setUniform3f("DirDirectection", dirRotation);
	m_AddSSAO->setUniform3f("DirColour", m_DirectionalColour);
	
	m_AddSSAO->setUniform3f("viewPos", camPos);
	m_AddSSAO->setUniformMat4("View", View);
	m_AddSSAO->setUniformMat4("Proj", P);
	m_AddSSAO->setUniformMat4("invView", glm::inverse(View));
	m_AddSSAO->setUniformMat4("invProj", glm::inverse(P));
	m_AddSSAO->setUniform3f("AmbientColor", glm::vec3(m_SceneAmbient.x(), m_SceneAmbient.y(), m_SceneAmbient.z()));
	m_AddSSAO->setUniform1f("AmbientInten", m_fAmbientInten);

	for (int i = 0; i < m_Lights.size(); i++)
	{
		std::string light = "lights[" + std::to_string(i) + "]";

		if (m_Lights[i]->toDelete)
		{
			m_AddSSAO->setUniform1f((light + ".Inten").c_str(), 0);
			m_AddSSAO->setUniform3f((light + ".Color").c_str(), glm::vec3(0, 0, 0));
			delete m_Lights[i];
			m_Lights.erase(m_Lights.begin() + i);
			i--;
			continue;
		}
		m_AddSSAO->setUniform3f((light + ".Pos").c_str() , m_Lights[i]->Pos);
		m_AddSSAO->setUniform3f((light + ".Color").c_str(), m_Lights[i]->Color);
		m_AddSSAO->setUniform1f((light + ".Radius").c_str(), m_Lights[i]->Radius);
		m_AddSSAO->setUniform1f((light + ".Inten").c_str(), m_Lights[i]->Intencity);

		m_AddSSAO->setUniform1f((light + ".Angle").c_str(), m_Lights[i]->Angle);
		glm::quat rotation = glm::toQuat(glm::orientate4(glm::radians(m_Lights[i]->Rot)));
		m_AddSSAO->setUniform3f((light + ".Direction").c_str(), glm::mat3(View) * (glm::vec3(0, 1, 0)* rotation));
	}
	
	m_EnginePointer->m_Window->Clear();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	//Unbind texture so the active texture IDs can be reused
	/*glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE7); glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE8); glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE9); glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE10); glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE11); glBindTexture(GL_TEXTURE_2D, 0);*/

	
	//m_Reflection->Bind();
	glBindVertexArray(m_QuadVAO);
	glDisable(GL_DEPTH_TEST);
	m_ReflectionShader->enable();
	GLint albedoLoc = glGetUniformLocation(m_ReflectionShader->getID(), "gAlbedoSS");
	glUniform1i(albedoLoc, 12);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetTextureID());

	GLint baseImageLoc = glGetUniformLocation(m_ReflectionShader->getID(), "LightPass");
	glUniform1i(baseImageLoc, 13);
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, m_FrameBuffer->GetTextureID());
	//
	GLint posLoc = glGetUniformLocation(m_ReflectionShader->getID(), "gPositionSS");
	glUniform1i(posLoc, 14);
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetPos());
	//
	GLint normLoc = glGetUniformLocation(m_ReflectionShader->getID(), "gNormalSS");
	glUniform1i(normLoc, 15);
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetNormal());
	//
	GLint compLoc = glGetUniformLocation(m_ReflectionShader->getID(), "gComponentSS");
	glUniform1i(compLoc, 16);
	glActiveTexture(GL_TEXTURE16);
	glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetAdditional());
	GLint cubeLoc = glGetUniformLocation(m_ReflectionShader->getID(), "cubeTexture");
	glUniform1i(cubeLoc, 17);
	glActiveTexture(GL_TEXTURE17);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyBox->GetTexture());
	m_ReflectionShader->setUniform3f("viewPos", camPos);
	m_ReflectionShader->setUniformMat4("View", View);
	m_ReflectionShader->setUniformMat4("Proj", P);
	m_ReflectionShader->setUniformMat4("invView", glm::inverse(View));
	m_ReflectionShader->setUniformMat4("invProj", glm::inverse(P));
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_EnginePointer->m_Window->Clear();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	return;
	m_LumaBuffer->Bind();
	m_EnginePointer->m_Window->Clear();
	m_Reflection->Render();
	
	m_HBlurBuffer->GetShader()->enable();
	m_HBlurBuffer->GetShader()->setUniform1f("sigmaValue", m_fBloomSigma);
	m_HBlurBuffer->GetShader()->setUniform1i("kernalSize", m_iBloomKernalSize);
	m_HBlurBuffer->GetShader()->setUniform2f("resolution", glm::vec2(m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight()));
	m_HBlurBuffer->Bind();
	m_EnginePointer->m_Window->Clear();
	m_LumaBuffer->Render();

	m_VBlurBuffer->GetShader()->enable();
	m_VBlurBuffer->GetShader()->setUniform1f("sigmaValue", m_fBloomSigma);
	m_VBlurBuffer->GetShader()->setUniform1i("kernalSize", m_iBloomKernalSize);
	m_VBlurBuffer->GetShader()->setUniform2f("resolution", glm::vec2(m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight()));
	m_VBlurBuffer->Bind();
	m_EnginePointer->m_Window->Clear();
	m_HBlurBuffer->Render();

	m_Bloom->GetShader()->enable();
	m_Bloom->GetShader()->setUniform1f("intensity", m_fBloomIntensity);
	m_Bloom->Bind();
	m_EnginePointer->m_Window->Clear();
	m_VBlurBuffer->Render();

	m_Vignette->GetShader()->enable();
	m_Vignette->GetShader()->setUniform1f("RADIUS", m_fVignetteRadius);
	m_Vignette->GetShader()->setUniform1f("SOFTNESS", m_fVignetteSoftness);
	m_Vignette->GetShader()->setUniform2f("resolution", glm::vec2(m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight()));
	m_Vignette->Bind();
	m_EnginePointer->m_Window->Clear();
	m_Bloom->Render(m_Reflection->GetTextureID());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_Vignette->Render();


	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Engine::graphics::PostProcessingStack::SetUpUI()
{
	//Lighting
	m_SceneLighting = new nanogui::Window(m_EnginePointer->m_Window, "Scene Lighting");
	m_SceneLighting->setPosition(Eigen::Vector2i(15, 15));
	m_SceneLighting->setLayout(new nanogui::GroupLayout());

	m_SceneLighting->add<nanogui::Label>("Ambient Colour", "sans-bold", 15);
	nanogui::ColorPicker* picker = new nanogui::ColorPicker(m_SceneLighting, m_SceneAmbient);
	picker->setCallback([&](nanogui::Color color)
	{
		m_SceneAmbient = color;
	});
	picker->setHeight(5);

	m_SceneLighting->add<nanogui::Label>("Ambient Intensity", "sans-bold", 15);
	nanogui::Widget *panelLight = new nanogui::Widget(m_SceneLighting);
	panelLight->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 20));

	nanogui::Slider *slider = new nanogui::Slider(panelLight);
	slider->setValue(m_fAmbientInten);
	slider->setFixedWidth(150);
	nanogui::TextBox *textBox = new nanogui::TextBox(panelLight);
	textBox->setFixedSize(Eigen::Vector2i(60, 25));
	textBox->setValue(std::to_string((int)(m_fAmbientInten * 100)));
	textBox->setUnits("%");
	slider->setCallback([textBox](float value) {
		textBox->setValue(std::to_string((int)(value * 100)));
		//m_fAmbientInten = value;
	});
	slider->setFinalCallback([&](float value) {
		std::cout << "Final slider value: " << (int)(value * 100) << std::endl;
		m_fAmbientInten = value;
	});
	textBox->setFixedSize(Eigen::Vector2i(60, 25));
	textBox->setFontSize(20);
	textBox->setAlignment(nanogui::TextBox::Alignment::Right);

	nanogui::Button *addLight = new nanogui::Button(m_SceneLighting, "Add Light");
	addLight->setCallback([&]
	{
		m_Lights.push_back(new Light(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 5, 3));
	});

	nanogui::Button *deleteLight = new nanogui::Button(m_SceneLighting, "Remove Light");
	deleteLight->setCallback([&]
	{
		for (int i = 0; i < m_Lights.size(); i++)
		{
			if (m_SelectedLight == m_Lights[i])
			{
				m_Lights[i]->toDelete = true;
			}
		}
	});

	nanogui::ColorPicker* lightpicker = new nanogui::ColorPicker(m_SceneLighting, m_SceneAmbient);
	lightpicker->setCallback([&](nanogui::Color color)
	{
		if (m_SelectedLight != nullptr)
		{
			m_SelectedLight->Color = glm::vec3(color.x(), color.y(), color.z());
		}
	});
	lightpicker->setHeight(5);
	nanogui::Button *skyBoxBtn = new nanogui::Button(m_SceneLighting, "Select Skybox");
	skyBoxBtn->setCallback([&]
	{
		std::string s = FileUtils::BrowseFiles("Selected Skybox Texture");
		s = s.substr(0, s.find_last_of("\\/"));
		s += '/';
		std::cout << s.c_str() << std::endl;
		m_SkyBox->ChangeTexture(s);
	});

	m_SceneLighting->add<nanogui::Label>("Directional Light", "sans-bold", 15);
	nanogui::Widget *panelDirection = new nanogui::Widget(m_SceneLighting);
	panelDirection->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 20));

	nanogui::Slider *sliderDirInten = new nanogui::Slider(panelDirection);
	sliderDirInten->setValue(m_fDirectionalIntencity);
	sliderDirInten->setFixedWidth(150);
	nanogui::TextBox *textDirInten = new nanogui::TextBox(panelDirection);
	textDirInten->setFixedSize(Eigen::Vector2i(60, 25));
	textDirInten->setValue(std::to_string((int)m_fDirectionalIntencity*100));
	textDirInten->setUnits("%");
	sliderDirInten->setCallback([&, textDirInten](float value) {
		textDirInten->setValue(std::to_string((int)(value * 100)));
		m_fDirectionalIntencity = value;
	});
	sliderDirInten->setFinalCallback([&](float value) {
		m_fDirectionalIntencity = value;
	});
	textDirInten->setFixedSize(Eigen::Vector2i(60, 25));
	textDirInten->setFontSize(20);
	textDirInten->setAlignment(nanogui::TextBox::Alignment::Right);

	nanogui::Widget *panelDirRot = new nanogui::Widget(m_SceneLighting);
	panelDirRot->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 20));
	panelDirRot->add<nanogui::Label>("X", "sans-bold", 15);
	nanogui::Slider *sliderDirX = new nanogui::Slider(panelDirRot);
	sliderDirX->setValue(m_DirectionalX);
	sliderDirX->setFixedWidth(70);
	sliderDirX->setCallback([&, textDirInten](float value) {
		m_DirectionalX = value;
	});
	sliderDirX->setFinalCallback([&](float value) {
		m_DirectionalX = value;
	});
	panelDirRot->add<nanogui::Label>("Y", "sans-bold", 15);
	nanogui::Slider *sliderDirY = new nanogui::Slider(panelDirRot);
	sliderDirY->setValue(m_DirectionalY);
	sliderDirY->setFixedWidth(70);
	sliderDirY->setCallback([&, textDirInten](float value) {
		m_DirectionalY = value;
	});
	sliderDirY->setFinalCallback([&](float value) {
		m_DirectionalY = value;
	});

	nanogui::ColorPicker* pickerDir = new nanogui::ColorPicker(m_SceneLighting, nanogui::Color(m_DirectionalColour.x, m_DirectionalColour.y, m_DirectionalColour.z,1.0f));
	pickerDir->setCallback([&](nanogui::Color color)
	{
		m_DirectionalColour = glm::vec3(color.x(), color.y(), color.z());
	});
	pickerDir->setHeight(5);

	m_SceneLighting->add<nanogui::Label>("Cel Shading", "sans-bold", 15);
	nanogui::Widget *panelCellShading = new nanogui::Widget(m_SceneLighting);
	panelCellShading->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 20));
	CheckBox *csOutline = new CheckBox(panelCellShading, "Cel Shading",
		[&](bool state) { m_bCellShading = !m_bCellShading; }
	);
	csOutline->setChecked(m_bCellShading);
	nanogui::IntBox<int> *textBoxCS = new nanogui::IntBox<int>(panelCellShading);
	textBoxCS->setFixedSize(Eigen::Vector2i(60, 25));
	textBoxCS->setValue(m_iCells);
	textBoxCS->setFixedSize(Eigen::Vector2i(60, 25));
	textBoxCS->setFontSize(20);
	textBoxCS->setAlignment(nanogui::TextBox::Alignment::Right);
	textBoxCS->setEditable(true);
	textBoxCS->setCallback([&](int value) {
		m_iCells = value;
	});
	//Post Pro

	m_PostProWindow = new nanogui::Window(m_EnginePointer->m_Window, "Post Processing");
	m_PostProWindow->setPosition(Eigen::Vector2i(m_EnginePointer->m_Window->getWidth()- 200, 15));
	m_PostProWindow->setLayout(new nanogui::GroupLayout());

	//Bloom Intensity
	m_PostProWindow->add<nanogui::Label>("Bloom Intensity", "sans-bold", 15);
	nanogui::Widget *panel = new nanogui::Widget(m_PostProWindow);
	panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 20));

	nanogui::Slider *sliderBloom = new nanogui::Slider(panel);
	sliderBloom->setValue(m_fBloomIntensity);
	sliderBloom->setFixedWidth(150);
	nanogui::TextBox *textBoxBloom = new nanogui::TextBox(panel);
	textBoxBloom->setFixedSize(Eigen::Vector2i(60, 25));
	textBoxBloom->setValue(std::to_string((int)(m_fBloomIntensity * 100)));
	textBoxBloom->setUnits("%");
	sliderBloom->setCallback([&, textBoxBloom](float value) {
		textBoxBloom->setValue(std::to_string((int)(value * 100)));
		m_fBloomIntensity = value*2;
	});
	sliderBloom->setFinalCallback([&](float value) {
		m_fBloomIntensity = value*2;
	});
	textBoxBloom->setFixedSize(Eigen::Vector2i(60, 25));
	textBoxBloom->setFontSize(20);
	textBoxBloom->setAlignment(nanogui::TextBox::Alignment::Right);

	//Bloom Sigma
	nanogui::Widget *sigmaValue = new nanogui::Widget(m_PostProWindow);
	sigmaValue->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	sigmaValue->setSize(nanogui::Vector2i(100, 20));
	sigmaValue->add<nanogui::Label>("Bloom Sigma", "sans-bold", 15);
	nanogui::FloatBox<float>* sigma = new nanogui::FloatBox<float>(sigmaValue, m_fBloomSigma);
	sigma->setCallback([&](float value) {
		m_fBloomSigma = value;
	});
	sigma->setEditable(true);

	//Bloom Kernal
	nanogui::Widget *kernalSize = new nanogui::Widget(m_PostProWindow);
	kernalSize->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	kernalSize->setSize(nanogui::Vector2i(100, 20));
	kernalSize->add<nanogui::Label>("Bloom Kernal Size", "sans-bold", 15);
	nanogui::IntBox<int>* kernal = new nanogui::IntBox<int>(kernalSize, m_iBloomKernalSize);
	kernal->setCallback([&](int value) {
		m_iBloomKernalSize = value;
	});
	kernal->setEditable(true);

	//Vignette Radius
	m_PostProWindow->add<nanogui::Label>("Vignette", "sans-bold", 15);
	nanogui::Widget *vignettePanel = new nanogui::Widget(m_PostProWindow);
	vignettePanel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 20));

	nanogui::Slider *radiusSlider = new nanogui::Slider(vignettePanel);
	radiusSlider->setValue(m_fVignetteRadius);
	radiusSlider->setFixedWidth(150);
	nanogui::TextBox *vignetteTextBox = new nanogui::TextBox(vignettePanel);
	vignetteTextBox->setFixedSize(Eigen::Vector2i(60, 25));
	vignetteTextBox->setValue(std::to_string((int)(m_fVignetteRadius * 100)));
	vignetteTextBox->setUnits("%");
	radiusSlider->setCallback([&, vignetteTextBox](float value) {
		vignetteTextBox->setValue(std::to_string((int)(value * 100)));
		m_fVignetteRadius = value;
	});
	radiusSlider->setFinalCallback([&](float value) {
		m_fVignetteRadius = value;
	});
	vignetteTextBox->setFixedSize(Eigen::Vector2i(60, 25));
	vignetteTextBox->setFontSize(20);
	vignetteTextBox->setAlignment(nanogui::TextBox::Alignment::Right);

	//Vignette Softness
	nanogui::Widget *vignetteSoftPanel = new nanogui::Widget(m_PostProWindow);
	vignetteSoftPanel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 20));

	nanogui::Slider *softnessSlider = new nanogui::Slider(vignetteSoftPanel);
	softnessSlider->setValue(m_fVignetteSoftness);
	softnessSlider->setFixedWidth(150);
	nanogui::TextBox *vignetteSoftTextBox = new nanogui::TextBox(vignetteSoftPanel);
	vignetteSoftTextBox->setFixedSize(Eigen::Vector2i(60, 25));
	vignetteSoftTextBox->setValue(std::to_string((int)(m_fVignetteSoftness * 100)));
	vignetteSoftTextBox->setUnits("%");
	softnessSlider->setCallback([&, vignetteSoftTextBox](float value) {
		vignetteSoftTextBox->setValue(std::to_string((int)(value * 100)));
		m_fVignetteSoftness = value;
	});
	softnessSlider->setFinalCallback([&](float value) {
		m_fVignetteSoftness = value;
	});
	vignetteSoftTextBox->setFixedSize(Eigen::Vector2i(60, 25));
	vignetteSoftTextBox->setFontSize(20);
	vignetteSoftTextBox->setAlignment(nanogui::TextBox::Alignment::Right);

	CheckBox *cbMM = new CheckBox(m_PostProWindow, "MSAA x4",
		[&](bool state) { m_MS = !m_MS; }
	);
	cbMM->setChecked(m_MS);
	CheckBox *cbSSAO = new CheckBox(m_PostProWindow, "SSAO",
		[&](bool state) { m_SSAO = !m_SSAO; }
	);
	cbSSAO->setChecked(m_SSAO);
	CheckBox *cbOutline = new CheckBox(m_PostProWindow, "Outline",
		[&](bool state) { m_Outline = !m_Outline; }
	);
	cbOutline->setChecked(m_Outline);
}
