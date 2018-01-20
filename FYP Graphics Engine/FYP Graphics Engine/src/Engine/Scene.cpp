#include <Engine\Scene.h>

Engine::Scene::Scene(GLFWEngine* enginePointer)
{
	m_EnginePointer = enginePointer;
	V = glm::lookAt(
		camPos,
		glm::vec3(0, 5, 0),
		glm::vec3(0, 1, 0)
	);

	gui = new nanogui::FormHelper(enginePointer->m_Window);
	windowGUI = gui->addWindow(Eigen::Vector2i(10, 10), "Lighting");

	gui->addVariable("Ambient", m_SceneAmbient)
		->setFinalCallback([](const nanogui::Color &c) {
	});
	enginePointer->m_Window->setVisible(true);
	enginePointer->m_Window->performLayout();
	windowGUI->center();
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

		for (int i = 0; i < temp.size(); i++)
		{
			std::cout << temp[i].x << " " << temp[i].y << " " << temp[i].z << std::endl;
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
}

void Engine::Scene::AddObject(GameObject * obj)
{
	v_Objects.push_back(obj);
}
