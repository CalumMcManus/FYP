#include <Engine\project.h>

Engine::Project::Project(GLFWEngine* enginePointer)
{
	m_Scene = new Scene(enginePointer);
	m_EnginePointer = enginePointer;
	gui = new nanogui::FormHelper(enginePointer->m_Window);
	windowGUI = gui->addWindow(Eigen::Vector2i(10, 10), "NanoGUI Test");

	gui->addVariable("Camera Orbit", m_bOrbit, true);
	gui->addButton("Add Model", [&]()
	{
		AddModel();
	}
	)->setTooltip("Add model to the scene.");
	enginePointer->m_Window->setVisible(true);
	enginePointer->m_Window->performLayout();
	windowGUI->center();
}

bool Engine::Project::SetUpProjectDirectories()
{
	m_Directory = std::string(FileUtils::BrowseFolder());
	if (m_Directory == "")
	{
		//Failed to get directory
		std::cout << "ERROR: Project: SetUpProjectDirectories: Failed to get new project directory." << std::endl;
		return false;
	}
	std::string currentDirectory = m_Directory;
	if (!CreateConfigFile(currentDirectory))
		return false;

	FileUtils::CreateFolder(currentDirectory, "Assets");
	
	currentDirectory += "Assets/";
	FileUtils::CreateFolder(currentDirectory, "Models");
	FileUtils::CreateFolder(currentDirectory, "Textures");
	FileUtils::CreateFolder(currentDirectory, "Shaders");


	return false;
}

void Engine::Project::AddModel()
{
	GameObject* tempObj = new GameObject();
	tempObj->addComponent(new ModelRenderer(FileUtils::BrowseFiles("Select Model").c_str()));
	tempObj->addComponent(new Texture(FileUtils::BrowseFiles("Select Albedo Texture").c_str()));
	tempObj->addComponent(new Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	m_Scene->AddObject(tempObj);
}

void Engine::Project::Update()
{
	m_Scene->Update(m_bOrbit);
	m_Scene->Render();
}

void Engine::Project::Save()
{
	m_Scene->Save(m_Directory);
}

bool Engine::Project::CreateConfigFile(std::string path)
{
	//TODO:: Add error catching
	std::string filePath = path + "project.config";
	std::ofstream outputFile;
	outputFile.open(filePath);

	outputFile << "#Project Config" << std::endl;
	outputFile << "#Directory " << path << std::endl;
	
	outputFile.close();

	return true;
}
