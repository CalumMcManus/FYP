#include <Engine\Scene.h>

Engine::Scene::Scene(GLFWEngine* enginePointer, bool load)
{
	m_EnginePointer = enginePointer;
	

	V = glm::lookAt(
		camPos,
		glm::vec3(0, 5, 0),
		glm::vec3(0, 1, 0)
	);
	m_TransformWindow = new UI::TransformWindow(enginePointer);
	m_MaterialWindow = new UI::MaterialWindow(enginePointer);
	m_DefaultShader->enable();

	m_PostProcessing = new graphics::PostProcessingStack(enginePointer, load);
	
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
							m_MaterialWindow->SelectMaterial(v_Objects[j]->getComponent<Material>());
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
		Components::Material* tempMat = v_Objects[i]->getComponent<Components::Material>();
		
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
		if (tempMat)
			tempMat->BindTextures();
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
	std::string filePath = savePath + "save.txt";
	std::ofstream outputFile;
	outputFile.open(filePath, std::ofstream::out | std::ofstream::trunc);

	for (unsigned int i = 0; i < v_Objects.size(); i++)
	{
		outputFile << "#GOBEGIN" << std::endl;
		Components::Transform* tempTransform = v_Objects[i]->getComponent<Components::Transform>();
		Components::ModelRenderer* tempModel = v_Objects[i]->getComponent<Components::ModelRenderer>();
		Components::Material* tempMat = v_Objects[i]->getComponent<Components::Material>();

		if (tempTransform)
		{
			glm::vec3 pos = tempTransform->getPosition();
			glm::vec3 rot = tempTransform->getRotation();
			glm::vec3 scale = tempTransform->getSize();
			outputFile << "#Transform" << std::endl;
			outputFile << "Pos " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) << std::endl;
			outputFile << "Rot " + std::to_string(rot.x) + " " + std::to_string(rot.y) + " " + std::to_string(rot.z) << std::endl;
			outputFile << "Scale " + std::to_string(scale.x) + " " + std::to_string(scale.y) + " " + std::to_string(scale.z) << std::endl;
		}
		if (tempMat)
		{
			tempMat->Save(outputFile);
		}
		if (tempModel)
		{
			outputFile << "#Model" << std::endl;
			outputFile << tempModel->Path() << std::endl;
		}
		outputFile << "#GOEND" << std::endl;	
	}

	m_PostProcessing->Save(outputFile);
	outputFile.close();
}

void Engine::Scene::Load(std::string loadPath)
{
	std::string filePath = loadPath + "save.txt";
	ifstream inputFile;
	inputFile.open(filePath);

	std::string line;
	std::cout << "Loading..." << std::endl;
	if (inputFile.is_open())
	{
		while (!inputFile.eof())
		{
			std::getline(inputFile, line);
			std::string s;
			std::istringstream iss(line);

			iss >> s;

			if (s == "#GOBEGIN")
			{
				GameObject* obj = new GameObject();
				std::cout << "gameobject" << std::endl;
				while (s != "#GOEND")
				{
					std::getline(inputFile, line);
					iss = std::istringstream(line);
					iss >> s;
					if (s == "#Transform")
					{
						float f;
						Transform* tf = new Transform();
						vec3 pos;
						vec3 rot;
						vec3 scale;

						std::getline(inputFile, line);
						iss = std::istringstream(line);
						iss >> s;
						iss >> f; pos.x = f;
						iss >> f; pos.y = f;
						iss >> f; pos.z = f;

						std::getline(inputFile, line);
						iss = std::istringstream(line);
						iss >> s;
						iss >> f; rot.x = f;
						iss >> f; rot.y = f;
						iss >> f; rot.z = f;

						std::getline(inputFile, line);
						iss = std::istringstream(line);
						iss >> s;
						iss >> f; scale.x = f;
						iss >> f; scale.y = f;
						iss >> f; scale.z = f;

						tf->setPosition(pos);
						tf->setRotation(rot);
						tf->setSize(scale);

						obj->addComponent(tf);
					}
					if (s == "#Material")
					{
						float f;
						std::string path = "";
						Material* mat = new Material(m_DefaultShader);
						glm::vec3 color;
						std::getline(inputFile, line);
						iss = std::istringstream(line);
						iss >> s;
						iss >> f; color.x = f;
						iss >> f; color.y = f;
						iss >> f; color.z = f;
						mat->SetColour(color);

						std::getline(inputFile, line);
						if(line.size() > 0)
							mat->AddAlbedo(line.c_str());

						std::getline(inputFile, line);
						if (line.size() > 0)
							mat->AddSpecular(line.c_str());

						std::getline(inputFile, line);
						if (line.size() > 0)
							mat->AddNormal(line.c_str());

						obj->addComponent(mat);

					}
					if (s == "#Model")
					{
						ModelRenderer* render = nullptr;

						std::getline(inputFile, line);

						render = new ModelRenderer(line.c_str());

						obj->addComponent(render);
					}
				}
				v_Objects.push_back(obj);
			}

			if (s == "#Lighting")
				m_PostProcessing->Load(inputFile);
		}
	}
	else
	{
		std::cout << "Scene: Load: Failed to open file" << std::endl;
	}

}
