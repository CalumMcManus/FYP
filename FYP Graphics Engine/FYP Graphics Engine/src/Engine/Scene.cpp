#include <Engine\Scene.h>

Engine::Scene::Scene()
{
	V = glm::lookAt(
		camPos,
		glm::vec3(0, 5, 0),
		glm::vec3(0, 1, 0)
	);
}

Engine::Scene::~Scene()
{
}

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

	m_SkyBox->Draw(P, V * glm::translate(camPos));

	m_DefaultShader->enable();
	m_DefaultShader->setUniformMat4("P", P);
	m_DefaultShader->setUniformMat4("V", V );
	m_DefaultShader->setUniform3f("light_pos", glm::vec3(1, 1, -2));
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
