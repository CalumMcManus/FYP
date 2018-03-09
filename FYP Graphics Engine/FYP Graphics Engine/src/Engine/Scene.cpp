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

	m_PostProcessing = new graphics::PostProcessingStack(enginePointer);
	
	m_LightObject = new GameObject();
	m_LightObject->addComponent(new ModelRenderer("../Assets/Models/Light.obj"));

	

	
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
	//Selection
	if (glfwGetMouseButton(&m_EnginePointer->m_Window->getGLFWWindow(), 0) == GLFW_PRESS && glfwGetMouseButton(&m_EnginePointer->m_Window->getGLFWWindow(), 0) != GLFW_REPEAT)
	{
		glm::dvec2 mousePos;
		glfwGetCursorPos(&m_EnginePointer->m_Window->getGLFWWindow(), &mousePos.x, &mousePos.y);
		std::vector<glm::vec3> temp = Physics::RayCast::CastRay(camPos, mousePos, 20, P, V, m_EnginePointer->m_Window);
		bool selected = false;
		
		for (int i = 0; i < temp.size(); i++)
		{
			//std::cout << temp[i].x << " " << temp[i].y << " " << temp[i].z << std::endl;
			if (!(glfwGetKey(&m_EnginePointer->m_Window->getGLFWWindow(), GLFW_KEY_TAB) == GLFW_PRESS))
			{
				for (int j = 0; j < v_Objects.size(); j++)
				{
					Transform* transform = v_Objects[j]->getComponent<Transform>();
					if (transform != nullptr)
					{
						glm::vec3 checkVector = transform->getPosition() - temp[i];
						float mag = sqrt(pow(checkVector.x, 2) + pow(checkVector.y, 2) + pow(checkVector.z, 2));
						if (mag < 2)
						{
							std::cout << "Selected Model!" << std::endl;
							m_TransformWindow->SelectTransform(transform);
							selected = true;
							break;
						}
					}

				}
			}
			else
			{
				for (int l = 0; l < m_PostProcessing->Lights().size(); l++)
				{
					graphics::PostProcessingStack::Light* light = m_PostProcessing->Lights()[l];
					glm::vec3 checkVector = light->Pos - temp[i];
					float mag = sqrt(pow(checkVector.x, 2) + pow(checkVector.y, 2) + pow(checkVector.z, 2));
					if (mag < 2)
					{
						std::cout << "Selected Light!" << std::endl;
						m_TransformWindow->SelectLight(light);
						m_PostProcessing->SetSelectedLight(light);
						selected = true;
						break;
					}

				}
			}
			if (selected)break;
		}
		if (!selected)
		{
			//m_TransformWindow->Clear();
		}
	}
	//Render
	m_PostProcessing->Bind();

	m_SkyBox->Draw(P, V * glm::translate(camPos));
	m_DefaultShader->enable();

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

			m_DefaultShader->setUniformMat4("Model", tempTrasnform->getMatrix());
		}
		if (tempTexture)
			tempTexture->bindTexture(m_DefaultShader);
		if (tempModel)
			tempModel->getModel().render();
	}
	//std::cout << m_TransformWindow->IsMouseOver() << std::endl;
	if (glfwGetKey(&m_EnginePointer->m_Window->getGLFWWindow(), GLFW_KEY_TAB) == GLFW_PRESS)
	{
		glDepthMask(0);
		m_LightShader->enable();
		for (int l = 0; l < m_PostProcessing->Lights().size(); l++)
		{
			glm::mat4 M = glm::translate(m_PostProcessing->Lights()[l]->Pos);
			glm::mat4 MVP = P * V * M;
			glm::mat4 MV = V * M;
			m_LightShader->setUniformMat4("MVP", MVP);
			m_LightShader->setUniformMat4("ModelViewMatrix", MV);
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(MV));
			m_LightShader->setUniformMat3("NormalMatrix", normalMatrix);

			m_LightShader->setUniformMat4("Model", M);
			m_LightShader->setUniform3f("color", m_PostProcessing->Lights()[l]->Color);

			m_LightObject->getComponent<ModelRenderer>()->getModel().render();
		}
		glDepthMask(1);
	}
	m_PostProcessing->Render(P, V, camPos);
	
}

void Engine::Scene::AddObject(GameObject * obj)
{
	v_Objects.push_back(obj);
}

void Engine::Scene::Save(std::string savePath)
{
}
