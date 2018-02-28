#include <Engine\Engine.h>
#include <Engine\project.h>

Engine::GLFWEngine::GLFWEngine(const char* projectName, bool isNew)
{
	m_Window = new graphics::Window(projectName, 1280, 720);
	m_Project = new Project(this);

	if (isNew)
	{
		m_Project->SetUpProjectDirectories();
	}
}

Engine::GLFWEngine::~GLFWEngine()
{
}

void Engine::GLFWEngine::Run()
{
	while (!m_Window->Closed())
	{
		m_Window->Clear();
		m_Window->drawContents();
		m_Project->Update();
		m_Window->drawWidgets();
		m_Window->Update();
	}
}

void Engine::GLFWEngine::Resize()
{
}
