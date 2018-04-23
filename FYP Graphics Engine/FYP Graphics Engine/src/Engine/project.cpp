#include <Engine\project.h>

Engine::Project::Project(GLFWEngine* enginePointer)
{
	m_EnginePointer = enginePointer;
	
	projectgui = new nanogui::FormHelper(m_EnginePointer->m_Window);
	projectSetup = projectgui->addWindow(Eigen::Vector2i(10, 10), "Project");
	projectgui->addButton("Load Project", [&]()
	{
		m_bLoad = true;
		if(Init())
			projectSetup->setVisible(false);
	}
	)->setTooltip("Load a project");
	projectgui->addButton("New Project", [&]()
	{
		if(Init())
			projectSetup->setVisible(false);
	}
	)->setTooltip("Start a new project");
	m_EnginePointer->m_Window->setVisible(true);
	m_EnginePointer->m_Window->performLayout();
	projectSetup->center();
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
	//FileUtils::CreateFolder(currentDirectory, "Shaders");


	return false;
}

void Engine::Project::AddModel()
{
	GameObject* tempObj = new GameObject();
	tempObj->addComponent(new Material(new graphics::Shader("../Assets/Shaders/differed.vert", "../Assets/Shaders/differed.frag")));

	std::string modelPath = FileUtils::BrowseFiles("Select Model");
	std::set<char> delims{ '\\', '/' };
	std::string fileName = FileUtils::splitpath(modelPath, delims).back();
	std::string newPath = m_Directory + "Assets/Models/" + fileName;
	std::cout << newPath << std::endl;
	//glfwRestoreWindow(m_EnginePointer->m_Window->getGLFWWindow());
	if (FileUtils::Exists(newPath))
	{
		tempObj->addComponent(new ModelRenderer(newPath.c_str()));
	}
	else
	{
		//Copy file over
		FileUtils::TransferFile(modelPath, newPath);
		tempObj->addComponent(new ModelRenderer(newPath.c_str()));
	}

	
	tempObj->addComponent(new Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	m_Scene->AddObject(tempObj);
	
}

void Engine::Project::Update()
{
	if (m_Scene)
	{
		m_Scene->Update(m_bOrbit);
		m_Scene->Render();
	}
}

void Engine::Project::Save()
{
	m_Scene->Save(m_Directory);
}

void Engine::Project::Load()
{
	m_Scene->Load(m_Directory);
}

bool Engine::Project::Init()
{
	if (m_bLoad)
	{
		m_Directory = FileUtils::BrowseFiles("Select Save File");
		if (m_Directory == "") return false;
		m_Directory = m_Directory.substr(0, m_Directory.find_last_of("\\/"));
		m_Directory += '/';
	}
	m_Scene = new Scene(m_EnginePointer, m_bLoad);
	gui = new nanogui::FormHelper(m_EnginePointer->m_Window);
	
	if (m_bLoad)
	{
		std::cout << m_Directory << std::endl;
		Load();
	}
	else
	{
		if (!SetUpProjectDirectories())
			return false;
	}

	windowGUI = gui->addWindow(Eigen::Vector2i(m_EnginePointer->m_Window->getWidth() - 150, 400), "Utility");

	gui->addButton("Add Model", [&]()
	{
		AddModel();
	}
	)->setTooltip("Add model to the scene.");
	gui->addButton("Remove Model", [&]()
	{
		m_Scene->DeleteObject();
	}
	)->setTooltip("Add model to the scene.");
	gui->addButton("Save", [&]()
	{
		Save();
	}
	)->setTooltip("Save Project");

	m_EnginePointer->m_Window->setVisible(true);
	m_EnginePointer->m_Window->performLayout();
	//windowGUI->center();
	return true;
}

bool Engine::Project::CreateConfigFile(std::string path)
{
	//TODO:: Add error catching
	std::string filePath = path + "project.config";
	std::ofstream outputFile;
	outputFile.open(filePath, std::ofstream::out | std::ofstream::trunc);

	outputFile << "#Project Config" << std::endl;
	outputFile << "#Directory " << path << std::endl;
	
	outputFile.close();

	return true;
}
