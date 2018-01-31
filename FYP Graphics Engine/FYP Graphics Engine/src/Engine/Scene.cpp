#include <Engine\Scene.h>

Engine::Scene::Scene(GLFWEngine* enginePointer)
{
	m_EnginePointer = enginePointer;
	V = glm::lookAt(
		camPos,
		glm::vec3(0, 5, 0),
		glm::vec3(0, 1, 0)
	);

	m_FrameBuffer = new graphics::FrameBuffer(enginePointer->m_Window);

	gui = new nanogui::FormHelper(enginePointer->m_Window);
	windowGUI = gui->addWindow(Eigen::Vector2i(10, 10), "Lighting");

	gui->addVariable("Ambient", m_SceneAmbient)
		->setFinalCallback([](const nanogui::Color &c) {
	});

	


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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Transform Window
	m_TransformWindow = new nanogui::Window(enginePointer->m_Window, "Transform");
	m_TransformWindow->setPosition(Eigen::Vector2i(15, 55));
	m_TransformWindow->setLayout(new nanogui::GroupLayout());

	//Position
	m_TransformWindow->add<nanogui::Label>("Position", "sans-bold", 15);
	nanogui::Widget *pos = new nanogui::Widget(m_TransformWindow);
	pos->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	pos->setSize(nanogui::Vector2i(100, 20));
	pos->add<nanogui::Label>("X", "sans-bold", 15);
	posX = new nanogui::FloatBox<float>(pos, 0);
	posX->setCallback([&](float value) {
		if (selectedTransform != nullptr)
		{
			selectedTransform->setPosition(glm::vec3(value, selectedTransform->getPosition().y, selectedTransform->getPosition().z));
			std::cout << selectedTransform->getPosition().x << " " << selectedTransform->getPosition().y << " " << selectedTransform->getPosition().z << std::endl;
		}
	});
	posX->setEditable(true);
	pos->add<nanogui::Label>("Y", "sans-bold", 15);
	posY = new nanogui::FloatBox<float>(pos, 0);
	posY->setCallback([&](float value) {
		if (selectedTransform != nullptr)
		{
			selectedTransform->setPosition(glm::vec3(selectedTransform->getPosition().x, value, selectedTransform->getPosition().z));
			std::cout << selectedTransform->getPosition().x << " " << selectedTransform->getPosition().y << " " << selectedTransform->getPosition().z << std::endl;
		}
	});
	posY->setEditable(true);
	pos->add<nanogui::Label>("Z", "sans-bold", 15);
	posZ = new nanogui::FloatBox<float>(pos, 0);
	posZ->setCallback([&](float value) {
		if (selectedTransform != nullptr)
		{
			selectedTransform->setPosition(glm::vec3(selectedTransform->getPosition().x, selectedTransform->getPosition().y, value));
			std::cout << selectedTransform->getPosition().x << " " << selectedTransform->getPosition().y << " " << selectedTransform->getPosition().z << std::endl;
		}
	});
	posZ->setEditable(true);

	//Rotation
	m_TransformWindow->add<nanogui::Label>("Rotation", "sans-bold", 15);
	nanogui::Widget *rot = new nanogui::Widget(m_TransformWindow);
	rot->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	rot->setSize(nanogui::Vector2i(100, 20));
	rot->add<nanogui::Label>("X", "sans-bold", 15);
	rotX = new nanogui::FloatBox<float>(rot, 0);
	rotX->setCallback([&](float value) {
		if (selectedTransform != nullptr)
		{
			selectedTransform->setRotation(glm::vec3(value, selectedTransform->getRotation().y, selectedTransform->getRotation().z));
			std::cout << selectedTransform->getRotation().x << " " << selectedTransform->getRotation().y << " " << selectedTransform->getRotation().z << std::endl;
		}
	});
	rotX->setEditable(true);
	rot->add<nanogui::Label>("Y", "sans-bold", 15);
	rotY = new nanogui::FloatBox<float>(rot, 0);
	rotY->setCallback([&](float value) {
		if (selectedTransform != nullptr)
		{
			selectedTransform->setRotation(glm::vec3(selectedTransform->getRotation().x, value, selectedTransform->getRotation().z));
			std::cout << selectedTransform->getRotation().x << " " << selectedTransform->getRotation().y << " " << selectedTransform->getRotation().z << std::endl;
		}
	});
	rotY->setEditable(true);
	rot->add<nanogui::Label>("Z", "sans-bold", 15);
	rotZ = new nanogui::FloatBox<float>(rot, 0);
	rotZ->setCallback([&](float value) {
		if (selectedTransform != nullptr)
		{
			selectedTransform->setRotation(glm::vec3(selectedTransform->getRotation().x, selectedTransform->getRotation().y, value));
			std::cout << selectedTransform->getRotation().x << " " << selectedTransform->getRotation().y << " " << selectedTransform->getRotation().z << std::endl;
		}
	});
	rotZ->setEditable(true);

	//Scale
	m_TransformWindow->add<nanogui::Label>("Scale", "sans-bold", 15);
	nanogui::Widget *scale = new nanogui::Widget(m_TransformWindow);
	scale->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	scale->setSize(nanogui::Vector2i(100, 20));
	scale->add<nanogui::Label>("X", "sans-bold", 15);
	scaleX = new nanogui::FloatBox<float>(scale, 0.f);
	scaleX->setCallback([&](float value) {
		if (selectedTransform != nullptr)
		{
			selectedTransform->setSize(glm::vec3(value, selectedTransform->getSize().y, selectedTransform->getSize().z));
			std::cout << selectedTransform->getSize().x << " " << selectedTransform->getSize().y << " " << selectedTransform->getSize().z << std::endl;
		}
	});
	scaleX->setEditable(true);
	scale->add<nanogui::Label>("Y", "sans-bold", 15);
	scaleY = new nanogui::FloatBox<float>(scale, 0.f);
	scaleY->setCallback([&](float value) {
		if (selectedTransform != nullptr)
		{
			selectedTransform->setSize(glm::vec3(selectedTransform->getSize().x, value, selectedTransform->getSize().z));
			std::cout << selectedTransform->getSize().x << " " << selectedTransform->getSize().y << " " << selectedTransform->getSize().z << std::endl;
		}
	});
	scaleY->setEditable(true);
	scale->add<nanogui::Label>("Z", "sans-bold", 15);
	scaleZ = new nanogui::FloatBox<float>(scale, 0.f);
	scaleZ->setCallback([&](float value) {
		if (selectedTransform != nullptr)
		{
			selectedTransform->setSize(glm::vec3(selectedTransform->getSize().x, selectedTransform->getSize().y, value));
			std::cout << selectedTransform->getSize().x << " " << selectedTransform->getSize().y << " " << selectedTransform->getSize().z << std::endl;
		}
	});
	scaleZ->setEditable(true);


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
						selectedTransform = transform;
						posX->setValue(transform->getPosition().x);
						posY->setValue(transform->getPosition().y);
						posZ->setValue(transform->getPosition().z);

						rotX->setValue(transform->getRotation().x);
						rotY->setValue(transform->getRotation().y);
						rotZ->setValue(transform->getRotation().z);

						scaleX->setValue(transform->getSize().x);
						scaleY->setValue(transform->getSize().y);
						scaleZ->setValue(transform->getSize().z);
						selected = true;
						break;
					}
				}

			}
			if (selected)break;
		}
	}

	m_SkyBox->Draw(P, V * glm::translate(camPos));

	m_DefaultShader->enable();
	m_DefaultShader->setUniformMat4("P", P);
	m_DefaultShader->setUniformMat4("V", V );
	m_DefaultShader->setUniform3f("light_pos", glm::vec3(1, 1, -2));
	m_DefaultShader->setUniform3f("light_ambient", glm::vec3(m_SceneAmbient.x(), m_SceneAmbient.y(), m_SceneAmbient.z()));
	//m_DefaultShader->setUniform3f("light_ambient", tempColor);
	//m_DefaultShader->setUniform3f("ray_dir", test);
	m_DefaultShader->setUniform3f("cam_pos", camPos);

	m_FrameBuffer->Bind();
	for (int i = 0; i < v_Objects.size(); i++)
	{
		//TODO:: Texture and other map handling needs to be moved into material
		Components::Transform* tempTrasnform = v_Objects[i]->getComponent<Components::Transform>();
		Components::ModelRenderer* tempModel = v_Objects[i]->getComponent<Components::ModelRenderer>();
		Components::Texture* tempTexture = v_Objects[i]->getComponent<Components::Texture>();
		
		if(tempTrasnform)
			m_DefaultShader->setUniformMat4("M", tempTrasnform->getMatrix());
		
		if (tempTexture)
			tempTexture->bindTexture(m_DefaultShader);
		if (tempModel)
			tempModel->getModel().render();
	}
	m_FrameBuffer->Render();
}

void Engine::Scene::AddObject(GameObject * obj)
{
	v_Objects.push_back(obj);
}
