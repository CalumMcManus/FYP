#include <Engine\Engine.h>
#include <Engine\project.h>

Engine::GLFWEngine::GLFWEngine(const char* projectName, bool isNew)
{
	m_Window = new graphics::Window(projectName, 1600, 900);
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
		if(!glfwGetKey(m_Window->getGLFWWindow(), GLFW_KEY_LEFT_SHIFT))
			m_Window->drawWidgets();
		if (glfwGetKey(m_Window->getGLFWWindow(), GLFW_KEY_ESCAPE))
			break;
		m_Window->Update();
	}
}

void Engine::GLFWEngine::Resize()
{
}
