#include <Engine\Graphics\postProcessingStack.h>

Engine::graphics::PostProcessingStack::PostProcessingStack(GLFWEngine * enginePointer)
{
	m_EnginePointer = enginePointer;
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
		float scale = (float)i / 64.0;
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

		
	m_MSBuffer = new graphics::GBuffer(m_EnginePointer->m_Window, 4, m_NoFilter);
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
		m_MSBuffer->Bind();
	else
		m_SSBuffer->Bind();
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, drawBuffers);
	m_EnginePointer->m_Window->Clear();
}

void Engine::graphics::PostProcessingStack::Render(glm::mat4 P)
{
	if (m_MS)
	{
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
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_SSBuffer->GetBufferID());
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			0, 0, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(),
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);
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

	m_FrameBufferAO->Bind();
	glBindVertexArray(m_QuadVAO);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	m_HBlurBufferAO->Bind();
	m_HBlurBuffer->GetShader()->enable();
	m_HBlurBuffer->GetShader()->setUniform1f("sigmaValue", 3);
	m_HBlurBuffer->GetShader()->setUniform1i("kernalSize", 25);	
	m_EnginePointer->m_Window->Clear();
	m_FrameBufferAO->Render();
	
	m_VBlurBufferAO->Bind();
	m_VBlurBufferAO->GetShader()->enable();
	m_VBlurBufferAO->GetShader()->setUniform1f("sigmaValue", 3);
	m_VBlurBufferAO->GetShader()->setUniform1i("kernalSize", 25);	
	m_EnginePointer->m_Window->Clear();
	m_HBlurBufferAO->Render();
	
	m_FinalBlueAO->Bind();
	m_EnginePointer->m_Window->Clear();
	m_VBlurBufferAO->Render();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_EnginePointer->m_Window->Clear();
	m_FinalBlueAO->Render();
	
	//return;
	//GLenum drawBuffersTwo[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	//glDrawBuffers(1, drawBuffersTwo);
	m_EnginePointer->m_Window->Clear();
		
	//Unbind();
	m_FrameBuffer->Bind();
	glBindVertexArray(m_QuadVAO);
	glDisable(GL_DEPTH_TEST);
	//m_SSAOShader->enable();
	m_AddSSAO->enable();
	if (m_SSAO)
	{
		GLint baseImageLoc = glGetUniformLocation(m_AddSSAO->getID(), "texFramebuffer");
		glUniform1i(baseImageLoc, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetTextureID());
		GLint ssaoLoc = glGetUniformLocation(m_AddSSAO->getID(), "ssaoTex");
		glUniform1i(ssaoLoc, 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_FinalBlueAO->GetTextureID());
			
	}
	else
	{
		m_NoFilter->enable();
		GLint baseImageLoc = glGetUniformLocation(m_NoFilter->getID(), "texFramebuffer");
		glUniform1i(baseImageLoc, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_SSBuffer->GetTextureID());
		
	}

	
	
	glDrawArrays(GL_TRIANGLES, 0, 6);


	m_LumaBuffer->Bind();
	m_EnginePointer->m_Window->Clear();
	m_FrameBuffer->Render();
	
	m_HBlurBuffer->GetShader()->enable();
	m_HBlurBuffer->GetShader()->setUniform1f("sigmaValue", m_fBloomSigma);
	m_HBlurBuffer->GetShader()->setUniform1i("kernalSize", m_iBloomKernalSize);
	m_HBlurBuffer->Bind();
	m_EnginePointer->m_Window->Clear();
	m_LumaBuffer->Render();

	m_VBlurBuffer->GetShader()->enable();
	m_VBlurBuffer->GetShader()->setUniform1f("sigmaValue", m_fBloomSigma);
	m_VBlurBuffer->GetShader()->setUniform1i("kernalSize", m_iBloomKernalSize);
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
	m_Vignette->Bind();
	m_EnginePointer->m_Window->Clear();
	m_Bloom->Render(m_FrameBuffer->GetTextureID());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_Vignette->Render();
}



void Engine::graphics::PostProcessingStack::SetUpUI()
{
	m_PostProWindow = new nanogui::Window(m_EnginePointer->m_Window, "Post Processing");
	m_PostProWindow->setPosition(Eigen::Vector2i(m_EnginePointer->m_Window->getWidth()- 200, 15));
	m_PostProWindow->setLayout(new nanogui::GroupLayout());

	//Bloom Intensity
	m_PostProWindow->add<nanogui::Label>("Bloom Intensity", "sans-bold", 15);
	nanogui::Widget *panel = new nanogui::Widget(m_PostProWindow);
	panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 20));

	nanogui::Slider *slider = new nanogui::Slider(panel);
	slider->setValue(0.5f);
	slider->setFixedWidth(150);
	nanogui::TextBox *textBox = new nanogui::TextBox(panel);
	textBox->setFixedSize(Eigen::Vector2i(60, 25));
	textBox->setValue("50");
	textBox->setUnits("%");
	slider->setCallback([&, textBox](float value) {
		textBox->setValue(std::to_string((int)(value * 100)));
		m_fBloomIntensity = value*2;
	});
	slider->setFinalCallback([&](float value) {
		m_fBloomIntensity = value*2;
	});
	textBox->setFixedSize(Eigen::Vector2i(60, 25));
	textBox->setFontSize(20);
	textBox->setAlignment(nanogui::TextBox::Alignment::Right);

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
	radiusSlider->setValue(0.5f);
	radiusSlider->setFixedWidth(150);
	nanogui::TextBox *vignetteTextBox = new nanogui::TextBox(vignettePanel);
	vignetteTextBox->setFixedSize(Eigen::Vector2i(60, 25));
	vignetteTextBox->setValue("50");
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
	softnessSlider->setValue(0.5f);
	softnessSlider->setFixedWidth(150);
	nanogui::TextBox *vignetteSoftTextBox = new nanogui::TextBox(vignetteSoftPanel);
	vignetteSoftTextBox->setFixedSize(Eigen::Vector2i(60, 25));
	vignetteSoftTextBox->setValue("50");
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

	CheckBox *cbMM = new CheckBox(m_PostProWindow, "MSAA x16",
		[&](bool state) { m_MS = !m_MS; }
	);
	CheckBox *cbSSAO = new CheckBox(m_PostProWindow, "SSAO",
		[&](bool state) { m_SSAO = !m_SSAO; }
	);
}
