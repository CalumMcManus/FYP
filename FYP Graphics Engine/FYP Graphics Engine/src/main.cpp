#include <iostream>

#include <Engine\window.h>
#include <Util\fileutil.h>

#include <Engine\Graphics\shader.h>
#include <Engine\Graphics\Mesh.h>
#include <Engine\gameObject.h>
#include <Component\modelRenderer.h>
#include <Component\material.h>
#include <Component\texture.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <Engine\project.h>

#include <gtx\transform.hpp>
#include <Engine\Graphics\SkyBox.h>
#include <nanogui\nanogui.h>

#include <windows.h>


#include <string>

#include <Engine/Engine.h>


using namespace std;



int main()
{
	Engine::GLFWEngine* engine = new Engine::GLFWEngine("FYP Graphics Engine", false);
	engine->Run();
	//Engine::graphics::Window *window = new Engine::graphics::Window("FYP Graphics Engine", 1270, 720);

	//cout << Engine::FileUtils::FileSize("../Assets/Models/test.obj") << endl;

	//graphics::Shader* defaultShader = new graphics::Shader("../Assets/Shaders/diffuse.vert", "../Assets/Shaders/diffuse.frag");
	////graphics::Shader* defaultShader = new graphics::Shader("../Assets/Shaders/diffuseOutline.vert", "../Assets/Shaders/diffuseOutline.frag");
	////graphics::Shader* defaultShader = new graphics::Shader("../Assets/Shaders/dsa.vert", "../Assets/Shaders/dsa.frag");
	//
	//glm::mat4 P = glm::perspective(1.16937f, (float)1280 / (float)720, 0.1f, 150.f);//1.16937 = 67 degrees
	//glm::mat4 V = glm::lookAt(glm::vec3(0, 7, -20), glm::vec3(0, 7, 0), glm::vec3(0, 1, 0));
	//glm::mat4 M = glm::translate(glm::vec3(0, 0, 0));// *glm::rotate(0, glm::vec3(0, 0, 0)) * glm::scale(glm::vec3(1, 1, 1));
	//
	//GameObject testObj;
	//testObj.addComponent(new ModelRenderer(Engine::FileUtils::BrowseFiles().c_str()));
	////testObj.addComponent(new ModelRenderer("../Assets/Models/test.obj"));

	//float rotationTest = 0;

	////First Pass
	//
	//defaultShader->enable();

	//defaultShader->setUniformMat4("P", P);
	//defaultShader->setUniformMat4("V", V);
	//defaultShader->setUniformMat4("M", M);

	//defaultShader->setUniform3f("Kd", vec3(0.86f, 0.19f, 0.1f)); // diffuse
	//defaultShader->setUniform3f("Ka", vec3(0.46f, 0.29f, 0.0f)); // am
	//defaultShader->setUniform3f("Ks", vec3(0.5f, 0.5f, 0.5f)); // spec

	//defaultShader->setUniform3f("La", vec3(0.4f, 0.4f, 0.4f));
	//defaultShader->setUniform3f("Ld", vec3(0.6f, 0.6f, 0.6f));
	//defaultShader->setUniform3f("Ls", vec3(0.2f, 0.2f, 0.2f));

	//
	//mat4 MV = M * V;
	////defaultShader->setUniform4f("colour", glm::vec4(1, 0, 0, 1));
	//defaultShader->setUniform3f("light_pos", glm::vec3(1, 1, -2));

	////Testure Stuff
	//testObj.addComponent(new Texture("../Assets/Textures/test.tga"));
	//testObj.getComponent<Texture>()->bindTexture(defaultShader);
	//testObj.addComponent(new Material(defaultShader));
	//testObj.getComponent<Material>()->ReadShader();

	//Engine::graphics::SkyBox* sBox = new Engine::graphics::SkyBox("../Assets/Textures/Cubemap/", "");


	//
	///*path = Engine::FileUtils::BrowseFiles();
	//std::cout << path << std::endl;*/
	//
	////Orbit Camera position with bounce
	//float theta = 0;
	//float step = 0.005f;
	//float radius = 15.f;
	//float x = 0;
	//float y = 0;

	//float rotationStep = step * (180 / 3.141);

	//bool yAxis = true;
	//float fYAxis = 0;

	//

	//vec3 camPos = glm::vec3(x, fYAxis, y);

	//Engine::Project *project = new Project();
	////Nanogui test

	//window->setBackground(nanogui::Color(0, 0, 0, 0));

	//nanogui::FormHelper *gui = new nanogui::FormHelper(window);
	//nanogui::ref<nanogui::Window> windowGUI = gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");
	//bool b = true;
	//string s = "Hello";
	//gui->addGroup("Basic types");
	//gui->addVariable("bool", b, true);
	//gui->addVariable("string", s);

	//gui->addButton("Create Project", [&]() 
	//{
	//	std::string path = Engine::FileUtils::BrowseFolder();
	//	std::cout << "Creating project directories at: " << path << std::endl;
	//	project->SetUpProjectDirectories();
	//}
	//)->setTooltip("Create a new project at directory.");;

	//window->setVisible(true);
	//window->performLayout();
	//windowGUI->center();
	//

	//while (!window->Closed())
	//{
	//	//rotationTest += 0.001f;
	//	
	//	x = radius*cos(theta);
	//	y = radius*sin(theta);
	//	theta += step;
	//	if (yAxis)
	//	{
	//		fYAxis -= step*10;
	//		if (fYAxis < -4)
	//			yAxis = !yAxis;
	//	}
	//	else
	//	{
	//		fYAxis += step*10;
	//		if (fYAxis > 4)
	//			yAxis = !yAxis;
	//	}
	//	camPos = glm::vec3(x, fYAxis+5, y);

	//	V = glm::lookAt(
	//		camPos,
	//		glm::vec3(0, 5, 0),
	//		glm::vec3(0, 1, 0)
	//	);

	//	window->Clear();
	//	
	//	//Skybox
	//	glDisable(GL_BLEND);
	//	sBox->Draw(P, V * glm::translate(camPos));

	//	//Dagger
	//	
	//	defaultShader->enable();
	//	
	//	glm::mat4 M = glm::translate(glm::vec3(0, 0, 0));// *glm::rotate(rotationTest, glm::vec3(0, 1, 0));
	//	defaultShader->setUniformMat4("M", M);
	//	defaultShader->setUniformMat4("V", V);
	//	MV = M * V;
	//	

	//	defaultShader->setUniformMat3("NormalMatrix", mat3(MV[0], MV[1], MV[2]));

	//	//Call this before drawing 3D world
	//	window->drawContents();
	//	testObj.getComponent<Texture>()->bindTexture(defaultShader);
	//	testObj.getComponent<ModelRenderer>()->getModel().render();

	//	
	//	
	//	//Call this to drawUI
	//	window->drawWidgets();
	//	window->Update();


	//}
	glfwTerminate();
	return 0;
}
