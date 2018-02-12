#include <Engine\Graphics\postProcessingStack.h>

Engine::graphics::PostProcessingStack::PostProcessingStack(GLFWEngine * enginePointer)
{
	m_EnginePointer = enginePointer;
	m_AABuffer = new graphics::FrameBuffer(enginePointer->m_Window, 4, new graphics::Shader("../Assets/Shaders/NoFilter.vert", "../Assets/Shaders/NoFilter.frag"));
	m_FrameBuffer = new graphics::FrameBuffer(enginePointer->m_Window, 1, new graphics::Shader("../Assets/Shaders/NoFilter.vert", "../Assets/Shaders/NoFilter.frag"));
	m_LumaBuffer = new graphics::FrameBuffer(enginePointer->m_Window, 1, new graphics::Shader("../Assets/Shaders/BrightOnly.vert", "../Assets/Shaders/BrightOnly.frag"));
	m_HBlurBuffer = new graphics::FrameBuffer(enginePointer->m_Window, 1, new graphics::Shader("../Assets/Shaders/Bloom.vert", "../Assets/Shaders/Bloom.frag"));
	m_VBlurBuffer = new graphics::FrameBuffer(enginePointer->m_Window, 1, new graphics::Shader("../Assets/Shaders/Bloom2.vert", "../Assets/Shaders/Bloom2.frag"));
	m_Vignette = new graphics::FrameBuffer(enginePointer->m_Window, 1, new graphics::Shader("../Assets/Shaders/Vignette.vert", "../Assets/Shaders/Vignette.frag"));
	m_Bloom = new graphics::CombineFilter(enginePointer->m_Window);

	SetUpUI();
}

Engine::graphics::PostProcessingStack::~PostProcessingStack()
{
}

void Engine::graphics::PostProcessingStack::Bind()
{
	m_AABuffer->Bind();
}

void Engine::graphics::PostProcessingStack::Render()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_AABuffer->GetBufferID()); // src FBO (multi-sample)
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FrameBuffer->GetBufferID());     // dst FBO (single-sample)
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glBlitFramebuffer(0, 0, 1280, 720, 
		0, 0, 1280, 720,             
		GL_COLOR_BUFFER_BIT,           
		GL_LINEAR);                    
									
	m_LumaBuffer->Bind();
	m_EnginePointer->m_Window->Clear();
	m_FrameBuffer->Render();

	m_HBlurBuffer->GetShader()->enable();
	m_HBlurBuffer->GetShader()->setUniform1f("sigmaValue", m_fBloomSigma);
	m_VBlurBuffer->GetShader()->setUniform1i("kernalSize", m_iBloomKernalSize);
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
}
