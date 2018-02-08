#include <iostream>
#include <Engine/Engine.h>

#include <cmath>
using namespace std;

int main()
{
	Engine::GLFWEngine* engine = new Engine::GLFWEngine("FYP Graphics Engine", false);
	engine->Run();
	glfwTerminate();
	return 0;
}
