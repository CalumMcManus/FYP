#include <Engine\Scene.h>

Engine::Scene::Scene(GLFWEngine* enginePointer)
{
	m_EnginePointer = enginePointer;
	V = glm::lookAt(
		camPos,
		glm::vec3(0, 5, 0),
		glm::vec3(0, 1, 0)
	);
	m_TransformWindow = new UI::TransformWindow(enginePointer);
	m_DefaultShader->enable();
	SetUpGBuffer();
	//m_PostProcessing = new graphics::PostProcessingStack(enginePointer);
	


	m_SceneLighting = new nanogui::Window(enginePointer->m_Window, "Scene Lighting");
	m_SceneLighting->setPosition(Eigen::Vector2i(15, 15));
	m_SceneLighting->setLayout(new nanogui::GroupLayout());

	new nanogui::Label(m_SceneLighting, "Ambient", "sans-bold", 20);
	m_SceneLighting->add<nanogui::Label>("Ambient Colour", "sans-bold", 15);
	nanogui::ColorPicker* picker = new nanogui::ColorPicker(m_SceneLighting, m_SceneAmbient);
	picker->setCallback([&](nanogui::Color color)
	{
		m_SceneAmbient = color;
	});
	picker->setHeight(5);
	
	m_SceneLighting->add<nanogui::Label>("Ambient Intensity", "sans-bold", 15);
	nanogui::Widget *panel = new nanogui::Widget(m_SceneLighting);
	panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 20));
	
	nanogui::Slider *slider = new nanogui::Slider(panel);
	slider->setValue(0.5f);
	slider->setFixedWidth(150);
	nanogui::TextBox *textBox = new nanogui::TextBox(panel);
	textBox->setFixedSize(Eigen::Vector2i(60, 25));
	textBox->setValue("50");
	textBox->setUnits("%");
	slider->setCallback([textBox](float value) {
		textBox->setValue(std::to_string((int)(value * 100)));
		//m_fAmbientInten = value;
	});
	slider->setFinalCallback([&](float value) {
		cout << "Final slider value: " << (int)(value * 100) << endl;
		m_fAmbientInten = value;
	});
	textBox->setFixedSize(Eigen::Vector2i(60, 25));
	textBox->setFontSize(20);
	textBox->setAlignment(nanogui::TextBox::Alignment::Right);

	
}

Engine::Scene::~Scene()
{
}
#include <Engine/Physics/RayCast.h>
void Engine::Scene::Update(bool orbit)
{
	
	if (!orbit)return;
	x = radius*cos(theta);
	y = radius*sin(theta);
	theta += step;
	if (yAxis)
	{
		fYAxis -= step * 10;
		if (fYAxis < -4)
			yAxis = !yAxis;
	}
	else
	{
		fYAxis += step * 10;
		if (fYAxis > 4)
			yAxis = !yAxis;
	}
	camPos = glm::vec3(x, fYAxis + 5, y);

	V = glm::lookAt(
		camPos,
		glm::vec3(0, 6, 0),
		glm::vec3(0, 1, 0)
	);
}

void Engine::Scene::Render()
{

	/*glm::dvec2 mousePos;
	glfwGetCursorPos(&m_EnginePointer->m_Window->getGLFWWindow(), &mousePos.x, &mousePos.y);
	glm::vec3 test = Physics::RayCast::ViewToWorldSpace(mousePos, P, V, m_EnginePointer->m_Window);

	glm::vec3 test2 = camPos + test * 15.f;
	glm::vec3 checkVector = glm::vec3(0, 0, 0) - test2;
	float mag = sqrt(pow(checkVector.x, 2) + pow(checkVector.y, 2) + pow(checkVector.z, 2));
	glm::vec3 tempColor = glm::vec3(1, 1, 1);
	if (mag < 5)
	{
		tempColor = glm::vec3(1, 0, 0);
		std::cout << "collide" << std::endl;
	}*/
	if (glfwGetMouseButton(&m_EnginePointer->m_Window->getGLFWWindow(), 0) == GLFW_PRESS && glfwGetMouseButton(&m_EnginePointer->m_Window->getGLFWWindow(), 0) != GLFW_REPEAT)
	{
		glm::dvec2 mousePos;
		glfwGetCursorPos(&m_EnginePointer->m_Window->getGLFWWindow(), &mousePos.x, &mousePos.y);
		std::vector<glm::vec3> temp = Physics::RayCast::CastRay(camPos, mousePos, 20, P, V, m_EnginePointer->m_Window);
		bool selected = false;
		for (int i = 0; i < temp.size(); i++)
		{
			//std::cout << temp[i].x << " " << temp[i].y << " " << temp[i].z << std::endl;
			for (int j = 0; j < v_Objects.size(); j++)
			{
				Transform* transform = v_Objects[j]->getComponent<Transform>();
				if (transform != nullptr)
				{
					glm::vec3 checkVector = transform->getPosition() - temp[i];
					float mag = sqrt(pow(checkVector.x, 2) + pow(checkVector.y, 2) + pow(checkVector.z, 2));
					if (mag < 5)
					{
						std::cout << "Selected!" << std::endl;
						m_TransformWindow->SelectTransform(transform);
						selected = true;
						break;
					}
				}

			}
			if (selected)break;
		}
	}
	//m_PostProcessing->Bind();
	glBindFramebuffer(GL_FRAMEBUFFER, m_DeferredFBO);
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, drawBuffers);
	m_EnginePointer->m_Window->Clear();
	//m_SkyBox->Draw(P, V * glm::translate(camPos));
	
	m_DefaultShader->enable();
	//m_DefaultShader->setUniformMat4("P", P);
	//m_DefaultShader->setUniformMat4("V", V );
	//m_DefaultShader->setUniform3f("light_pos", glm::vec3(1, 1, -2));
	//m_DefaultShader->setUniform3f("light_ambient", glm::vec3(m_SceneAmbient.x(), m_SceneAmbient.y(), m_SceneAmbient.z()));
	////m_DefaultShader->setUniform3f("light_ambient", tempColor);
	////m_DefaultShader->setUniform3f("ray_dir", test);
	//m_DefaultShader->setUniform3f("cam_pos", camPos);


	m_DefaultShader->setUniformMat4("Projection", P);
	for (int i = 0; i < v_Objects.size(); i++)
	{
		//TODO:: Texture and other map handling needs to be moved into material
		Components::Transform* tempTrasnform = v_Objects[i]->getComponent<Components::Transform>();
		Components::ModelRenderer* tempModel = v_Objects[i]->getComponent<Components::ModelRenderer>();
		Components::Texture* tempTexture = v_Objects[i]->getComponent<Components::Texture>();
		
		if (tempTrasnform)
		{
			glm::mat4 MVP = P * V * tempTrasnform->getMatrix();
			glm::mat4 MV = V * tempTrasnform->getMatrix();
			m_DefaultShader->setUniformMat4("MVP", MVP);
			m_DefaultShader->setUniformMat4("ModelViewMatrix", MV);
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(MV));
			m_DefaultShader->setUniformMat3("NormalMatrix", normalMatrix);

			//m_DefaultShader->setUniformMat4("M", tempTrasnform->getMatrix());
		}
		if (tempTexture)
			tempTexture->bindTexture(m_DefaultShader);
		if (tempModel)
			tempModel->getModel().render();
	}
	//m_PostProcessing->Render();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOFBO);
	drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	glClear(GL_COLOR_BUFFER_BIT);
	m_SSAOShader->enable();
	GLint posImageLoc = glGetUniformLocation(m_SSAOShader->getID(), "gPosition");
	glUniform1i(posImageLoc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Position);
	GLint normImageLoc = glGetUniformLocation(m_SSAOShader->getID(), "gNormal");
	glUniform1i(normImageLoc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_Normal);
	GLint noiseImageLoc = glGetUniformLocation(m_SSAOShader->getID(), "texNoise");
	glUniform1i(noiseImageLoc, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_NoiseTexture);
	
	glUniform3fv(glGetUniformLocation(m_SSAOShader->getID(), "samples"), 64,
		glm::value_ptr(ssaoKernel[0]));
	m_SSAOShader->setUniformMat4("projection", P);
	
	glBindVertexArray(m_QuadVAO);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_EnginePointer->m_Window->Clear();
	//Unbind();
	glBindVertexArray(m_QuadVAO);
	glDisable(GL_DEPTH_TEST);
	//m_SSAOShader->enable();

	m_ScreenShader->enable();
	GLint baseImageLoc = glGetUniformLocation(m_ScreenShader->getID(), "texFramebuffer");
	glUniform1i(baseImageLoc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_SSAOTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::Scene::AddObject(GameObject * obj)
{
	v_Objects.push_back(obj);
}
float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}
void Engine::Scene::SetUpGBuffer()
{
	//Set up FBO
	glGenFramebuffers(1, &m_DeferredFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_DeferredFBO);

	//Depth Buffer
	glGenRenderbuffers(1, &m_DepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight());

	createBuffer(GL_TEXTURE1, GL_RGB32F, m_Position); //Position
	//glActiveTexture(GL_TEXTURE1);
	//glGenTextures(1, &m_Position);
	//glBindTexture(GL_TEXTURE_2D, m_Position);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	createBuffer(GL_TEXTURE2, GL_RGB32F, m_Normal); //Normal
	createBuffer(GL_TEXTURE3, GL_RGB8, m_Color); //Color

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Position, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_Normal, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_Color, 0);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Frame Buffer: Contructor: Issue completing frame buffer with code " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

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

	glGenFramebuffers(1, &m_SSAOFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOFBO);

	glGenTextures(1, &m_SSAOTexture);
	glBindTexture(GL_TEXTURE_2D, m_SSAOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSAOTexture, 0);


}

void Engine::Scene::createBuffer(GLenum texUnit, GLenum format, GLuint & texid)
{
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexStorage2D(GL_TEXTURE_2D, 1, format, m_EnginePointer->m_Window->getWidth(), m_EnginePointer->m_Window->getHeight());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//glGenTextures(1, &texid);
	//glBindTexture(GL_TEXTURE_2D, texid);
	//glTexImage2D(GL_TEXTURE_2D, 0, format, 1280, 720, 0, GL_RGB, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
