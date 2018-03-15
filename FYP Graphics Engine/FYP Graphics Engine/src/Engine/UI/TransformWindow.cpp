#include <Engine\UI\TransformWindow.h>

Engine::UI::TransformWindow::TransformWindow(Engine::GLFWEngine* enginePointer)
{
	//Transform Window
	m_TransformWindow = new nanogui::Window(enginePointer->m_Window, "Transform");
	m_TransformWindow->setPosition(Eigen::Vector2i(15, 55));
	m_TransformWindow->setLayout(new nanogui::GroupLayout());

	//Position
	m_PosLable = new nanogui::Label(m_TransformWindow, "Position", "sans-bold", 15);
	
	nanogui::Widget *pos = new nanogui::Widget(m_TransformWindow);
	pos->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	pos->setSize(nanogui::Vector2i(100, 20));
	pos->add<nanogui::Label>("X", "sans-bold", 15);
	posX = new nanogui::FloatBox<float>(pos, 0);
	posX->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr && !m_bSelectedLight)
		{
			m_SelectedTransform->setPosition(glm::vec3(value, m_SelectedTransform->getPosition().y, m_SelectedTransform->getPosition().z));
			std::cout << m_SelectedTransform->getPosition().x << " " << m_SelectedTransform->getPosition().y << " " << m_SelectedTransform->getPosition().z << std::endl;
		}
		else if (m_SelectedLight != nullptr && m_bSelectedLight)
		{
			m_SelectedLight->Pos.x = value;
		}
	});
	posX->setEditable(true);
	pos->add<nanogui::Label>("Y", "sans-bold", 15);
	posY = new nanogui::FloatBox<float>(pos, 0);
	posY->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr && !m_bSelectedLight)
		{
			m_SelectedTransform->setPosition(glm::vec3(m_SelectedTransform->getPosition().x, value, m_SelectedTransform->getPosition().z));
			std::cout << m_SelectedTransform->getPosition().x << " " << m_SelectedTransform->getPosition().y << " " << m_SelectedTransform->getPosition().z << std::endl;
		}
		else if (m_SelectedLight != nullptr && m_bSelectedLight)
		{
			m_SelectedLight->Pos.y = value;
		}
	});
	posY->setEditable(true);
	pos->add<nanogui::Label>("Z", "sans-bold", 15);
	posZ = new nanogui::FloatBox<float>(pos, 0);
	posZ->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr && !m_bSelectedLight)
		{
			m_SelectedTransform->setPosition(glm::vec3(m_SelectedTransform->getPosition().x, m_SelectedTransform->getPosition().y, value));
			std::cout << m_SelectedTransform->getPosition().x << " " << m_SelectedTransform->getPosition().y << " " << m_SelectedTransform->getPosition().z << std::endl;
		}
		else if (m_SelectedLight != nullptr && m_bSelectedLight)
		{
			m_SelectedLight->Pos.z = value;
		}
	});
	posZ->setEditable(true);

	//Rotation
	m_RotLable = new nanogui::Label(m_TransformWindow, "Rotation", "sans-bold", 15);

	nanogui::Widget *rot = new nanogui::Widget(m_TransformWindow);
	rot->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	rot->setSize(nanogui::Vector2i(100, 20));
	rot->add<nanogui::Label>("X", "sans-bold", 15);
	rotX = new nanogui::FloatBox<float>(rot, 0);
	rotX->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr && !m_bSelectedLight)
		{
			m_SelectedTransform->setRotation(glm::vec3(value, m_SelectedTransform->getRotation().y, m_SelectedTransform->getRotation().z));
			std::cout << m_SelectedTransform->getRotation().x << " " << m_SelectedTransform->getRotation().y << " " << m_SelectedTransform->getRotation().z << std::endl;
		}
		else if (m_SelectedLight != nullptr && m_bSelectedLight)
		{
			m_SelectedLight->Rot.x = value;
		}
	});
	rotX->setEditable(true);
	rot->add<nanogui::Label>("Y", "sans-bold", 15);
	rotY = new nanogui::FloatBox<float>(rot, 0);
	rotY->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr && !m_bSelectedLight)
		{
			m_SelectedTransform->setRotation(glm::vec3(m_SelectedTransform->getRotation().x, value, m_SelectedTransform->getRotation().z));
			std::cout << m_SelectedTransform->getRotation().x << " " << m_SelectedTransform->getRotation().y << " " << m_SelectedTransform->getRotation().z << std::endl;
		}
		else if (m_SelectedLight != nullptr && m_bSelectedLight)
		{
			m_SelectedLight->Rot.y = value;
		}
	});
	rotY->setEditable(true);
	rot->add<nanogui::Label>("Z", "sans-bold", 15);
	rotZ = new nanogui::FloatBox<float>(rot, 0);
	rotZ->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr && !m_bSelectedLight)
		{
			m_SelectedTransform->setRotation(glm::vec3(m_SelectedTransform->getRotation().x, m_SelectedTransform->getRotation().y, value));
			std::cout << m_SelectedTransform->getRotation().x << " " << m_SelectedTransform->getRotation().y << " " << m_SelectedTransform->getRotation().z << std::endl;
		}
		else if (m_SelectedLight != nullptr && m_bSelectedLight)
		{
			m_SelectedLight->Rot.z = value;
		}
	});
	rotZ->setEditable(true);

	//Scale
	m_ScaleLable = new nanogui::Label(m_TransformWindow, "Scale", "sans-bold", 15);

	nanogui::Widget *scale = new nanogui::Widget(m_TransformWindow);
	scale->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
		nanogui::Alignment::Maximum, 0, 10));
	scale->setSize(nanogui::Vector2i(100, 20));

	m_ScaleXLable = new nanogui::Label(scale, "X", "sans-bold", 15);

	scaleX = new nanogui::FloatBox<float>(scale, 0.f);
	scaleX->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr && !m_bSelectedLight)
		{
			m_SelectedTransform->setSize(glm::vec3(value, m_SelectedTransform->getSize().y, m_SelectedTransform->getSize().z));
			std::cout <<  "New Scale: " << m_SelectedTransform->getSize().x << " " << m_SelectedTransform->getSize().y << " " << m_SelectedTransform->getSize().z << std::endl;
		}
		else if (m_SelectedLight != nullptr && m_bSelectedLight)
		{
			m_SelectedLight->Radius = value;
		}
	});
	scaleX->setEditable(true);
	m_ScaleYLable = new nanogui::Label(scale, "Y", "sans-bold", 15);
	scaleY = new nanogui::FloatBox<float>(scale, 0.f);
	scaleY->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr && !m_bSelectedLight)
		{
			m_SelectedTransform->setSize(glm::vec3(m_SelectedTransform->getSize().x, value, m_SelectedTransform->getSize().z));
			std::cout << m_SelectedTransform->getSize().x << " " << m_SelectedTransform->getSize().y << " " << m_SelectedTransform->getSize().z << std::endl;
		}
		else if (m_SelectedLight != nullptr && m_bSelectedLight)
		{
			m_SelectedLight->Angle = value;
		}
	});
	scaleY->setEditable(true);
	m_ScaleZLable = new nanogui::Label(scale, "Z", "sans-bold", 15);
	scaleZ = new nanogui::FloatBox<float>(scale, 0.f);
	scaleZ->setCallback([&](float value) {
		if (m_SelectedTransform != nullptr && !m_bSelectedLight)
		{
			m_SelectedTransform->setSize(glm::vec3(m_SelectedTransform->getSize().x, m_SelectedTransform->getSize().y, value));
			std::cout << m_SelectedTransform->getSize().x << " " << m_SelectedTransform->getSize().y << " " << m_SelectedTransform->getSize().z << std::endl;
		}
		else if (m_SelectedLight != nullptr && m_bSelectedLight)
		{
			m_SelectedLight->Intencity = value;
		}
	});
	scaleZ->setEditable(true);
}

void Engine::UI::TransformWindow::SelectTransform(Components::Transform * selectedTransform)
{
	m_bSelectedLight = false;
	m_SelectedTransform = selectedTransform;
	posX->setValue(m_SelectedTransform->getPosition().x);
	posY->setValue(m_SelectedTransform->getPosition().y);
	posZ->setValue(m_SelectedTransform->getPosition().z);

	m_RotLable->setCaption("Rotation");
	rotX->setValue(m_SelectedTransform->getRotation().x);
	rotY->setValue(m_SelectedTransform->getRotation().y);
	rotZ->setValue(m_SelectedTransform->getRotation().z);

	scaleX->setValue(m_SelectedTransform->getSize().x);
	scaleY->setValue(m_SelectedTransform->getSize().y);
	scaleZ->setValue(m_SelectedTransform->getSize().z);

	m_ScaleXLable->setCaption("X");
	m_ScaleYLable->setCaption("Y");
	m_ScaleZLable->setCaption("Z");
}

void Engine::UI::TransformWindow::SelectLight(graphics::PostProcessingStack::Light * selectedLight)
{
	m_SelectedLight = selectedLight;
	m_bSelectedLight = true;
	posX->setValue(selectedLight->Pos.x);
	posY->setValue(selectedLight->Pos.y);
	posZ->setValue(selectedLight->Pos.z);
	m_ScaleLable->setCaption("Settings");
	rotX->setValue(selectedLight->Rot.x);
	rotY->setValue(selectedLight->Rot.y);
	rotZ->setValue(selectedLight->Rot.z);

	scaleX->setValue(selectedLight->Radius);
	scaleY->setValue(selectedLight->Angle);
	scaleZ->setValue(selectedLight->Intencity);

	m_ScaleXLable->setCaption("R");
	m_ScaleYLable->setCaption("A");
	m_ScaleZLable->setCaption("I");
}

void Engine::UI::TransformWindow::Clear()
{
	m_SelectedTransform = nullptr;
	posX->setValue(0);
	posY->setValue(0);
	posZ->setValue(0);

	rotX->setValue(0);
	rotY->setValue(0);
	rotZ->setValue(0);

	scaleX->setValue(0);
	scaleY->setValue(0);
	scaleZ->setValue(0);
}
