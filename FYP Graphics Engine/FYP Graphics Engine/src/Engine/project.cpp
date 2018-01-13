#include <Engine\project.h>

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
